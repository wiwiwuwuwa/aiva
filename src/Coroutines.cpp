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
        volatile uintptr_t isLocked{};
        volatile uintptr_t handle{};
        volatile uintptr_t sysFiberHandle{};
        volatile uintptr_t usrFiberHandle{};
    };

    struct System final
    {
        volatile uintptr_t exitFlag{};
        Span<Thread> threads{};
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

    thread.sysFiberHandle = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
    if (!thread.sysFiberHandle)
        CheckNoEntry();

    if (!Intrin::AtomicExchange(&thread.isLocked, false))
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, false, false) == false)
    {
        while (Intrin::AtomicCompareExchange(&thread.isLocked, false, true) == true)
            Intrin::YieldProcessor();

        auto const usrFiberHandle = thread.usrFiberHandle;
        thread.usrFiberHandle = {};

        if (Intrin::AtomicExchange(&thread.isLocked, false) == false)
            CheckNoEntry();

        if (usrFiberHandle)
            WinApi::SwitchToFiber((void*)usrFiberHandle);
        else
            Intrin::YieldProcessor();
    }

    if (Intrin::AtomicExchange(&thread.isLocked, true) == true)
        CheckNoEntry();

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();
    thread.sysFiberHandle = {};

    return {};
}


void Coroutines::InitSystem()
{
    if (GSystem)
        CheckNoEntry();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();

    GSystem->threads = Memory::GetHeapAlloc().CreateArray<Thread>(GetNumberOfCores());

    for (auto i = size_t{}; i < GSystem->threads.GetSize(); i++)
    {
        auto& thread = GSystem->threads[i];

        thread.isLocked = true;
        thread.handle = (uintptr_t)WinApi::CreateThread({}, 16384, ThreadAction, (void*)i, {}, {});

        if (!thread.handle)
            CheckNoEntry();
    }
}


void Coroutines::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    if (Intrin::AtomicExchange(&GSystem->exitFlag, true))
        CheckNoEntry();

    for (auto i = GSystem->threads.GetSize(); i > 0; i--)
    {
        auto& thread = GSystem->threads[i - 1];

        if (WinApi::WaitForSingleObject((void*)thread.handle, WinApi::INFINITE) == WinApi::WAIT_FAILED)
            CheckNoEntry();

        thread.handle = {};
        thread.isLocked = false;
    }

    GSystem->threads = Memory::GetHeapAlloc().DeleteArray(GSystem->threads);

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    auto const usrFiberHandle = (uintptr_t)WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
    if (!usrFiberHandle)
        CheckNoEntry();

    auto i = size_t{};

    while (true)
    {
        auto& thread = GSystem->threads[i];
        i = (i + 1) % GSystem->threads.GetSize();

        if (Intrin::AtomicCompareExchange(&thread.isLocked, false, true) == true)
            continue;

        thread.usrFiberHandle = usrFiberHandle;

        if (Intrin::AtomicExchange(&thread.isLocked, false) == false)
            CheckNoEntry();

        break;
    }
}


void Coroutines::Yield()
{
    CheckNoEntry();
}


void Coroutines::Close()
{
    CheckNoEntry();
}
