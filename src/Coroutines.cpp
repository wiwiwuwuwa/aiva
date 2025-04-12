#include "Coroutines.hpp"

#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "Intrin.hpp"
#include "Memory.hpp"
#include "RawObject.hpp"
#include "Span.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines;


namespace
{
    struct ThreadContext final
    {
        volatile uintptr_t threadHandle{};
        volatile uintptr_t systemFiberHandle{};
        volatile uintptr_t externFiberHandle{};
        volatile uintptr_t deleteFiberHandle{};
    };

    struct System final
    {
        uint32_t tlsHandle{ WinApi::TLS_OUT_OF_INDEXES };
        Span<ThreadContext> threadContexts{};
        volatile uintptr_t exitFlag{};
    };
}
// namespace


static RawObject<System> GSystemObject{};
static System* GSystem{};


static size_t GetNumberOfCores()
{
    WinApi::SYSTEM_INFO systemInfo{};
    WinApi::GetSystemInfo(&systemInfo);

    return (size_t)systemInfo.dwNumberOfProcessors;
}


static uint32_t __stdcall ThreadAction(void *const threadIndex)
{
    auto& threadContext = GSystem->threadContexts[(size_t)threadIndex];

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, (void*)((size_t)threadIndex + 1)))
        CheckNoEntry();

    auto const systemFiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!systemFiberHandle)
        CheckNoEntry();

    if (Intrin::AtomicCompareExchange(&threadContext.systemFiberHandle, {}, systemFiberHandle) != uintptr_t{})
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, false, false) == false)
    {
        auto const externFiberHandle = Intrin::AtomicCompareExchange(&threadContext.externFiberHandle, {}, {});
        if (externFiberHandle)
            WinApi::SwitchToFiber((void*)externFiberHandle);

        auto const deleteFiberHandle = Intrin::AtomicExchange(&threadContext.deleteFiberHandle, {});
        if (deleteFiberHandle)
            WinApi::DeleteFiber((void*)deleteFiberHandle);

        Intrin::YieldProcessor();
    }

    if (Intrin::AtomicExchange(&threadContext.systemFiberHandle, {}) == uintptr_t{})
        CheckNoEntry();

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, nullptr))
        CheckNoEntry();

    return {};
}


static void InitTlsHandle()
{
    GSystem->tlsHandle = WinApi::TlsAlloc();
    if (GSystem->tlsHandle == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();
}


static void InitThreadContexts()
{
    GSystem->threadContexts = Memory::GetHeapAlloc().CreateArray<ThreadContext>(GetNumberOfCores());
    if (!GSystem->threadContexts)
        CheckNoEntry();

    for (auto i = size_t{}; i < GSystem->threadContexts.GetSize(); i++)
    {
        auto& threadContext = GSystem->threadContexts[i];

        threadContext.threadHandle = (uintptr_t)WinApi::CreateThread({}, 16384, ThreadAction, (void*)i, {}, {});
        if (!threadContext.threadHandle)
            CheckNoEntry();
    }

    for (auto i = size_t{}; i < GSystem->threadContexts.GetSize(); i++)
        while (Intrin::AtomicCompareExchange(&GSystem->threadContexts[i].systemFiberHandle, {}, {}) == uintptr_t{})
            Intrin::YieldProcessor();
}


static void InitExitFlag()
{
    if (Intrin::AtomicExchange(&GSystem->exitFlag, false) == true)
        CheckNoEntry();
}


static void ShutExitFlag()
{
    if (Intrin::AtomicExchange(&GSystem->exitFlag, true) == true)
        CheckNoEntry();
}


static void ShutThreadContexts()
{
    for (auto i = GSystem->threadContexts.GetSize(); i > 0; i--)
    {
        auto& threadContext = GSystem->threadContexts[i - 1];

        if (WinApi::WaitForSingleObject((void*)threadContext.threadHandle, WinApi::INFINITE) == WinApi::WAIT_FAILED)
            CheckNoEntry();

        threadContext.threadHandle = {};
    }

    GSystem->threadContexts = Memory::GetHeapAlloc().DeleteArray(GSystem->threadContexts);
    if (GSystem->threadContexts)
        CheckNoEntry();
}


static void ShutTlsHandle()
{
    if (!WinApi::TlsFree(GSystem->tlsHandle))
        CheckNoEntry();

    GSystem->tlsHandle = WinApi::TLS_OUT_OF_INDEXES;
}


void Coroutines::InitSystem()
{
    if (GSystem)
        CheckNoEntry();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();

    InitTlsHandle();
    InitThreadContexts();
    InitExitFlag();
}


void Coroutines::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    ShutExitFlag();
    ShutThreadContexts();
    ShutTlsHandle();

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    if (!GSystem)
        CheckNoEntry();

    auto const externFiberHandle = (uintptr_t)WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
    if (!externFiberHandle)
        CheckNoEntry();

    auto i = size_t{};

    while (true)
    {
        auto& threadContext = GSystem->threadContexts[i];
        i = (i + 1) % GSystem->threadContexts.GetSize();

        if (Intrin::AtomicCompareExchange(&threadContext.externFiberHandle, {}, externFiberHandle) == uintptr_t{})
            break;

        Intrin::YieldProcessor();
    }
}


void Coroutines::Yield()
{
    if (!GSystem)
        CheckNoEntry();

    auto const threadIndex = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle) - 1;
    if (threadIndex == (size_t)(-1))
        CheckNoEntry();

    auto& workerContext = GSystem->threadContexts[threadIndex];

    auto const externFiberHandle = Intrin::AtomicExchange(&workerContext.externFiberHandle, {});
    if (!externFiberHandle)
        CheckNoEntry();

    auto i = (threadIndex + 1) % GSystem->threadContexts.GetSize();

    while (true)
    {
        auto& threadContext = GSystem->threadContexts[i];
        i = (i + 1) % GSystem->threadContexts.GetSize();

        if (Intrin::AtomicCompareExchange(&threadContext.externFiberHandle, {}, externFiberHandle) == uintptr_t{})
            break;

        Intrin::YieldProcessor();
    }

    auto const systemFiberHandle = Intrin::AtomicCompareExchange(&workerContext.systemFiberHandle, {}, {});
    if (!systemFiberHandle)
        CheckNoEntry();

    WinApi::SwitchToFiber((void*)systemFiberHandle);
}


void Coroutines::Close()
{
    if (!GSystem)
        CheckNoEntry();

    auto const threadIndex = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle) - 1;
    if (threadIndex == (size_t)(-1))
        CheckNoEntry();

    auto& workerContext = GSystem->threadContexts[threadIndex];

    auto const externFiberHandle = Intrin::AtomicExchange(&workerContext.externFiberHandle, {});
    if (!externFiberHandle)
        CheckNoEntry();

    if (Intrin::AtomicExchange(&workerContext.deleteFiberHandle, externFiberHandle) != uintptr_t{})
        CheckNoEntry();

    auto const systemFiberHandle = Intrin::AtomicCompareExchange(&workerContext.systemFiberHandle, {}, {});
    if (!systemFiberHandle)
        CheckNoEntry();

    WinApi::SwitchToFiber((void*)systemFiberHandle);
}
