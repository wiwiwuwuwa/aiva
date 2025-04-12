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
    struct Thread final
    {
        volatile uintptr_t threadHandle{};
        volatile uintptr_t sysFiberHandle{};
        volatile uintptr_t usrFiberHandle{};
        volatile uintptr_t delFiberHandle{};
    };

    struct System final
    {
        uint32_t tlsHandle{ WinApi::TLS_OUT_OF_INDEXES };
        Span<Thread> threads{};
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
    auto& thread = GSystem->threads[(size_t)threadIndex];

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, (void*)((size_t)threadIndex + 1)))
        CheckNoEntry();

    auto const sysFiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!sysFiberHandle)
        CheckNoEntry();

    if (Intrin::AtomicCompareExchange(&thread.sysFiberHandle, {}, sysFiberHandle) != uintptr_t{})
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, false, false) == false)
    {
        auto const usrFiberHandle = Intrin::AtomicCompareExchange(&thread.usrFiberHandle, {}, {});
        if (usrFiberHandle)
            WinApi::SwitchToFiber((void*)usrFiberHandle);

        auto const delFiberHandle = Intrin::AtomicCompareExchange(&thread.delFiberHandle, {}, {});
        if (delFiberHandle)
            WinApi::DeleteFiber((void*)delFiberHandle);

        Intrin::YieldProcessor();
    }

    if (Intrin::AtomicExchange(&thread.sysFiberHandle, {}) == uintptr_t{})
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


static void InitThreads()
{
    GSystem->threads = Memory::GetHeapAlloc().CreateArray<Thread>(GetNumberOfCores());
    if (!GSystem->threads)
        CheckNoEntry();

    for (auto i = size_t{}; i < GSystem->threads.GetSize(); i++)
    {
        auto& thread = GSystem->threads[i];

        thread.threadHandle = (uintptr_t)WinApi::CreateThread({}, 16384, ThreadAction, (void*)i, {}, {});
        if (!thread.threadHandle)
            CheckNoEntry();
    }
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


static void ShutThreads()
{
    for (auto i = GSystem->threads.GetSize(); i > 0; i--)
    {
        auto& thread = GSystem->threads[i - 1];

        if (WinApi::WaitForSingleObject((void*)thread.threadHandle, WinApi::INFINITE) == WinApi::WAIT_FAILED)
            CheckNoEntry();

        thread.threadHandle = {};
    }

    GSystem->threads = Memory::GetHeapAlloc().DeleteArray(GSystem->threads);
    if (GSystem->threads)
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
    InitThreads();
    InitExitFlag();
}


void Coroutines::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    ShutExitFlag();
    ShutThreads();
    ShutTlsHandle();

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    if (!GSystem)
        CheckNoEntry();

    auto const usrFiberHandle = (uintptr_t)WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
    if (!usrFiberHandle)
        CheckNoEntry();

    auto i = size_t{};

    while (true)
    {
        auto& thread = GSystem->threads[i];
        i = (i + 1) % GSystem->threads.GetSize();

        if (Intrin::AtomicCompareExchange(&thread.sysFiberHandle, {}, {}) == uintptr_t{})
            continue;

        if (Intrin::AtomicCompareExchange(&thread.usrFiberHandle, {}, usrFiberHandle) == uintptr_t{})
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

    auto& worker = GSystem->threads[threadIndex];

    auto const usrFiberHandle = Intrin::AtomicExchange(&worker.usrFiberHandle, {});
    if (!usrFiberHandle)
        CheckNoEntry();

    auto i = (threadIndex + 1) % GSystem->threads.GetSize();

    while (true)
    {
        auto& thread = GSystem->threads[i];
        i = (i + 1) % GSystem->threads.GetSize();

        if (Intrin::AtomicCompareExchange(&thread.usrFiberHandle, {}, usrFiberHandle) == uintptr_t{})
            break;

        Intrin::YieldProcessor();
    }

    auto const sysFiberHandle = Intrin::AtomicCompareExchange(&worker.sysFiberHandle, {}, {});
    if (!sysFiberHandle)
        CheckNoEntry();

    WinApi::SwitchToFiber((void*)sysFiberHandle);
}


void Coroutines::Close()
{
    if (!GSystem)
        CheckNoEntry();

    auto const threadIndex = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle) - 1;
    if (threadIndex == (size_t)(-1))
        CheckNoEntry();

    auto& worker = GSystem->threads[threadIndex];

    auto const usrFiberHandle = Intrin::AtomicExchange(&worker.usrFiberHandle, {});
    if (!usrFiberHandle)
        CheckNoEntry();

    if (Intrin::AtomicExchange(&worker.delFiberHandle, usrFiberHandle) != uintptr_t{})
        CheckNoEntry();

    auto const sysFiberHandle = Intrin::AtomicCompareExchange(&worker.sysFiberHandle, {}, {});
    if (!sysFiberHandle)
        CheckNoEntry();

    WinApi::SwitchToFiber((void*)sysFiberHandle);
}
