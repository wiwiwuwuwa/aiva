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
    struct System final
    {
        volatile long m_exitFlag{};
        Span<uintptr_t> m_threads{};
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


#include "Console.hpp"
static uint32_t __stdcall ThreadAction(void *const)
{
    while (!InterlockedCompareExchange(&GSystem->m_exitFlag, 0L, 0L))
    {
        Console::Print(" da ");
    }

    return {};
}


void Coroutines::InitSystem()
{
    if (GSystem)
        CheckNoEntry();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();

    GSystem->m_threads = Memory::GetHeapAlloc().CreateArray<uintptr_t>(GetNumberOfCores());

    for (auto i = size_t{}; i < GSystem->m_threads.GetSize(); i++)
    {
        auto const threadHandle = (uintptr_t)WinApi::CreateThread({}, {}, ThreadAction, (void*)(uintptr_t)i, {}, {});
        if (!threadHandle)
            CheckNoEntry();

        GSystem->m_threads[i] = threadHandle;
    }
}


void Coroutines::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    if (InterlockedExchange(&GSystem->m_exitFlag, 1L))
        CheckNoEntry();

    auto const nCount = (uint32_t)GSystem->m_threads.GetSize();
    auto const lpHandles = (void const*const*const)&GSystem->m_threads.GetData();

    if (WinApi::WaitForMultipleObjects(nCount, lpHandles, true, WinApi::INFINITE) == WinApi::WAIT_FAILED)
        CheckNoEntry();

    GSystem->m_threads = Memory::GetHeapAlloc().DeleteArray(GSystem->m_threads);

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Coroutines::Start(CoroutineAction_t, uintptr_t const)
{
    CheckNoEntry();
}


void Coroutines::Yield()
{
    CheckNoEntry();
}
