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
    enum class ExitFlag : uintptr_t
    {
        kNone,
        kExit,
    };


    struct ThreadContext final
    {
        volatile uintptr_t threadHandle{};
        volatile uintptr_t fiberHandle{};
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
    auto systemInfo = WinApi::SYSTEM_INFO{};
    WinApi::GetSystemInfo(&systemInfo);

    return (size_t)systemInfo.dwNumberOfProcessors;
}


static uint32_t __stdcall ThreadAction(void *const threadIndex)
{
    auto& threadContext = GSystem->threadContexts[(size_t)threadIndex];

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, (void*)((size_t)threadIndex + 1)))
        CheckNoEntry();

    auto const fiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!fiberHandle)
        CheckNoEntry();

    Intrin::AtomicExchange(&threadContext.fiberHandle, fiberHandle);

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, {}, {}) != (uintptr_t)ExitFlag::kExit)
        Intrin::YieldProcessor();

    Intrin::AtomicExchange(&threadContext.fiberHandle, {});

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, {}))
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
        while (Intrin::AtomicCompareExchange(&GSystem->threadContexts[i].fiberHandle, {}, {}) == uintptr_t{})
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


void Coroutines::Spawn(CoroutineAction_t, uintptr_t const)
{
    CheckNoEntry();
}


void Coroutines::Yield()
{
    CheckNoEntry();
}


void Coroutines::Close()
{
    CheckNoEntry();
}
