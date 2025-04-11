#include "Threads.hpp"

#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Threads;


namespace
{
    struct ThreadContext final
    {
        ThreadAction_t action;
        uintptr_t userData{};

        void* winThread{};
    };
}
// namespace


static uint32_t ThreadAction(void *const userData)
{
    auto& context = (ThreadContext&)userData;

    context.action(context.userData);
    Memory::GetHeapAlloc().Delete(context);

    return {};
}


size_t Threads::GetNumberOfCores()
{
    WinApi::SYSTEM_INFO systemInfo{};
    WinApi::GetSystemInfo(&systemInfo);

    return (size_t)systemInfo.dwNumberOfProcessors;
}


void Threads::CreateThread(ThreadAction_t threadAction, uintptr_t const userData)
{
    auto& context = Memory::GetHeapAlloc().Create<ThreadContext>(threadAction, userData);

    context.winThread = WinApi::CreateThread({}, {}, ThreadAction, &context, {}, {});
    if (!context.winThread)
        CheckNoEntry();
}
