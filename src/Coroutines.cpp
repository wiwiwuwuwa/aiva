#include "Coroutines.hpp"

#include "Ensures.hpp"
#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "Queue.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines;


namespace
{
    class SystemContext final : public NonCopyable
    {
    public:
        SystemContext() = default;
        ~SystemContext() = default;

        void EnqueueUserFiber(void *const handle);
        bool HasAnyUserFiber() const;
        void* DequeueUserFiber();

    private:
        SpinLock m_userFibersLock;
        Queue<void*> m_userFibers;
    };
}
// namespace


void SystemContext::EnqueueUserFiber(void *const handle)
{
    SpinLockScope_t const lockScope{ m_userFibersLock };
    m_userFibers.Enqueue(handle);
}


bool SystemContext::HasAnyUserFiber() const
{
    SpinLockScope_t const lockScope{ const_cast<SpinLock&>(m_userFibersLock) };
    return !m_userFibers.IsEmpty();
}


void* SystemContext::DequeueUserFiber()
{
    SpinLockScope_t const lockScope{ m_userFibersLock };
    return m_userFibers.Dequeue();
}


static SpinLock GSystemContextLock{};
static ManageObject<SystemContext> GSystemContext{};


void Coroutines::InitSystem()
{
    SpinLockScope_t const lockScope{ GSystemContextLock };
    GSystemContext.Construct();
}


void Coroutines::ShutSystem()
{
    SpinLockScope_t const lockScope{ GSystemContextLock };
    GSystemContext.Destruct();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    auto const fiberHandle = WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
    if (!fiberHandle)
        CheckNoEntry();

    SpinLockScope_t const lockScope{ GSystemContextLock };
    GSystemContext->EnqueueUserFiber(fiberHandle);
}


void Coroutines::Yield()
{
    CheckNoEntry();
}


[[noreturn]] void Coroutines::Close()
{
    CheckNoEntry();
}


// #include "Coroutines.hpp"

// #include "Allocators.hpp"
// #include "Ensures.hpp"
// #include "Intrin.hpp"
// #include "LockScope.hpp"
// #include "ManageObject.hpp"
// #include "Memory.hpp"
// #include "Queue.hpp"
// #include "Span.hpp"
// #include "SpinLock.hpp"
// #include "WinApi.hpp"


// using namespace Aiva;
// using namespace Aiva::Coroutines;


// namespace
// {
//     enum class FiberState : uintptr_t
//     {
//         kYield,
//         kClose,
//     };


//     struct ThreadContext final
//     {
//         volatile uintptr_t thread{};
//         volatile uintptr_t fiber{};

//         volatile FiberState userState{};
//     };


//     struct System final
//     {
//         uint32_t tlsHandle{ WinApi::TLS_OUT_OF_INDEXES };
//         Span<ThreadContext> contexts{};

//         volatile uintptr_t exitFlag{};

//         SpinLock userFibersLock{};
//         Queue<uintptr_t> userFibers{};
//     };
// }
// // namespace


// static ManageObject<System> GSystemObject{};
// static System* GSystem{};


// static size_t GetNumberOfCores()
// {
//     auto systemInfo = WinApi::SYSTEM_INFO{};
//     WinApi::GetSystemInfo(&systemInfo);

//     return (size_t)systemInfo.dwNumberOfProcessors;
// }


// static uint32_t __stdcall ThreadAction(void *const threadIndex)
// {
//     auto& context = GSystem->contexts[(size_t)threadIndex];

//     if (!WinApi::TlsSetValue(GSystem->tlsHandle, (void*)((size_t)threadIndex + 1)))
//         CheckNoEntry();

//     auto const fiber = (uintptr_t)WinApi::ConvertThreadToFiber(nullptr);
//     if (!fiber)
//         CheckNoEntry();

//     Intrin::AtomicExchange(&context.fiber, fiber);

//     while (Intrin::AtomicCompareExchange(&GSystem->exitFlag, {}, {}) != uintptr_t{ 1 })
//     {
//         auto userFiber = uintptr_t{};

//         {
//             SpinLockScope_t const lock{ GSystem->userFibersLock };
//             userFiber = !GSystem->userFibers.IsEmpty() ? GSystem->userFibers.PopFront() : uintptr_t{};
//         }

//         if (!userFiber)
//         {
//             Intrin::YieldProcessor();
//             continue;
//         }

//         WinApi::SwitchToFiber((void*)userFiber);

//         if (Intrin::AtomicExchange((volatile uintptr_t*)&context.userState, {}) == (uintptr_t)FiberState::kClose)
//         {
//             Intrin::YieldProcessor();
//             continue;
//         }

//         {
//             SpinLockScope_t const lock{ GSystem->userFibersLock };
//             GSystem->userFibers.PushBack(userFiber);
//         }
//     }

//     Intrin::AtomicExchange(&context.fiber, {});

//     if (!WinApi::ConvertFiberToThread())
//         CheckNoEntry();

//     if (!WinApi::TlsSetValue(GSystem->tlsHandle, {}))
//         CheckNoEntry();

//     return {};
// }


// static void InitTlsHandle()
// {
//     GSystem->tlsHandle = WinApi::TlsAlloc();
//     if (GSystem->tlsHandle == WinApi::TLS_OUT_OF_INDEXES)
//         CheckNoEntry();
// }


// static void InitThreadContexts()
// {
//     GSystem->contexts = Memory::GetHeapAlloc().CreateArray<ThreadContext>(GetNumberOfCores());
//     if (!GSystem->contexts)
//         CheckNoEntry();

//     for (auto i = size_t{}; i < GSystem->contexts.GetSize(); i++)
//     {
//         auto& context = GSystem->contexts[i];

//         context.thread = (uintptr_t)WinApi::CreateThread({}, 16384, ThreadAction, (void*)i, {}, {});
//         if (!context.thread)
//             CheckNoEntry();
//     }

//     for (auto i = size_t{}; i < GSystem->contexts.GetSize(); i++)
//     {
//         auto& context = GSystem->contexts[i];

//         while (Intrin::AtomicCompareExchange(&context.fiber, {}, {}) == uintptr_t{})
//             Intrin::YieldProcessor();
//     }
// }


// static void ShutThreadContexts()
// {
//     for (auto i = GSystem->contexts.GetSize(); i > 0; i--)
//     {
//         auto& context = GSystem->contexts[i - 1];

//         if (WinApi::WaitForSingleObject((void*)context.thread, WinApi::INFINITE) == WinApi::WAIT_FAILED)
//             CheckNoEntry();

//         context.thread = {};
//     }

//     GSystem->contexts = Memory::GetHeapAlloc().DeleteArray(GSystem->contexts);
//     if (GSystem->contexts)
//         CheckNoEntry();
// }


// static void ShutTlsHandle()
// {
//     if (!WinApi::TlsFree(GSystem->tlsHandle))
//         CheckNoEntry();

//     GSystem->tlsHandle = WinApi::TLS_OUT_OF_INDEXES;
// }


// void Coroutines::InitSystem()
// {
//     if (GSystem)
//         CheckNoEntry();

//     GSystemObject.Construct();
//     GSystem = &GSystemObject.GetObject();

//     Intrin::AtomicExchange(&GSystem->exitFlag, uintptr_t{ 0 });
//     InitTlsHandle();
//     InitThreadContexts();
// }


// void Coroutines::ShutSystem()
// {
//     if (!GSystem)
//         CheckNoEntry();

//     Intrin::AtomicExchange(&GSystem->exitFlag, uintptr_t{ 1 });
//     ShutThreadContexts();
//     ShutTlsHandle();

//     GSystem = nullptr;
//     GSystemObject.Destruct();
// }


// void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
// {
//     if (!GSystem)
//         CheckNoEntry();

//     auto const userFiber = (uintptr_t)WinApi::CreateFiber(16384, (void*)coroutineAction, (void*)userData);
//     if (!userFiber)
//         CheckNoEntry();

//     SpinLockScope_t const lock{ GSystem->userFibersLock };
//     GSystem->userFibers.PushBack(userFiber);
// }


// void Coroutines::Yield()
// {
//     auto const threadNumber = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle);
//     if (!threadNumber)
//         CheckNoEntry();

//     auto& context = GSystem->contexts[threadNumber - 1];

//     Intrin::AtomicExchange((volatile uintptr_t*)&context.userState, (uintptr_t)FiberState::kYield);
//     WinApi::SwitchToFiber((void*)context.fiber);
// }


// [[noreturn]] void Coroutines::Close()
// {
//     auto const threadNumber = (size_t)WinApi::TlsGetValue(GSystem->tlsHandle);
//     if (!threadNumber)
//         CheckNoEntry();

//     auto& context = GSystem->contexts[threadNumber - 1];

//     Intrin::AtomicExchange((volatile uintptr_t*)&context.userState, (uintptr_t)FiberState::kClose);
//     WinApi::SwitchToFiber((void*)context.fiber);
// }
