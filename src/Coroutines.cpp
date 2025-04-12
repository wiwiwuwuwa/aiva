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

    if (!WinApi::TlsSetValue(GSystem->tlsHandle, threadIndex))
        CheckNoEntry();

    thread.sysFiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!thread.sysFiberHandle)
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, false, false) == false)
    {
        Intrin::YieldProcessor();
    }

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();
    thread.sysFiberHandle = {};

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
