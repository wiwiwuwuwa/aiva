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
    enum class UserFiberResult : uintptr_t
    {
        kYield,
        kClose,
    };


    struct ThreadContext final
    {
        volatile uintptr_t threadHandle{};
        volatile uintptr_t threadFiberHandle{};
        volatile uintptr_t currUserFiberHandle{};
        volatile uintptr_t currUserFiberResult{};
        volatile uintptr_t nextUserFiberHandle{};
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

    auto const threadFiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!threadFiberHandle)
        CheckNoEntry();

    Intrin::AtomicExchange(&threadContext.threadFiberHandle, threadFiberHandle);

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, false, false) == false)
    {
        auto const nextUserFiberHandle = Intrin::AtomicExchange(&threadContext.nextUserFiberHandle, {});
        auto const currUserFiberHandle = Intrin::AtomicExchange(&threadContext.currUserFiberHandle, nextUserFiberHandle);

        if (!currUserFiberHandle)
            continue;

        WinApi::SwitchToFiber((void*)currUserFiberHandle);
        auto const currUserFiberResult = (UserFiberResult)Intrin::AtomicCompareExchange(&threadContext.currUserFiberResult, {}, {});

        if (currUserFiberResult == UserFiberResult::kClose)
        {
            WinApi::DeleteFiber((void*)currUserFiberHandle);
            continue;
        }

        if (currUserFiberResult == UserFiberResult::kYield)
        {
            auto i = ((size_t)threadIndex + 1) % GSystem->threadContexts.GetSize();

            while (true)
            {
                auto& nextThreadContext = GSystem->threadContexts[i];
                i = (i + 1) % GSystem->threadContexts.GetSize();

                if (Intrin::AtomicCompareExchange(&nextThreadContext.nextUserFiberHandle, {}, currUserFiberHandle) == uintptr_t{})
                    break;
            }

            continue;
        }

        CheckNoEntry();
    }

    Intrin::AtomicExchange(&threadContext.threadFiberHandle, {});

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
        while (Intrin::AtomicCompareExchange(&GSystem->threadContexts[i].threadFiberHandle, {}, {}) == uintptr_t{})
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

    auto const userFiberHandle = (uintptr_t)WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
    if (!userFiberHandle)
        CheckNoEntry();

    auto i = size_t{};

    while (true)
    {
        auto& threadContext = GSystem->threadContexts[i];
        i = (i + 1) % GSystem->threadContexts.GetSize();

        if (Intrin::AtomicCompareExchange(&threadContext.nextUserFiberHandle, {}, userFiberHandle) == uintptr_t{})
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

    auto& threadContext = GSystem->threadContexts[threadIndex];

    auto const threadFiberHandle = Intrin::AtomicCompareExchange(&threadContext.threadFiberHandle, {}, {});
    if (!threadFiberHandle)
        CheckNoEntry();

    Intrin::AtomicExchange(&threadContext.currUserFiberResult, (uintptr_t)UserFiberResult::kYield);
    WinApi::SwitchToFiber((void*)threadFiberHandle);
}


void Coroutines::Close()
{
    if (!GSystem)
        CheckNoEntry();

    auto const threadIndex = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle) - 1;
    if (threadIndex == (size_t)(-1))
        CheckNoEntry();

    auto& threadContext = GSystem->threadContexts[threadIndex];

    auto const threadFiberHandle = Intrin::AtomicCompareExchange(&threadContext.threadFiberHandle, {}, {});
    if (!threadFiberHandle)
        CheckNoEntry();

    Intrin::AtomicExchange(&threadContext.currUserFiberResult, (uintptr_t)UserFiberResult::kClose);
    WinApi::SwitchToFiber((void*)threadFiberHandle);
}
