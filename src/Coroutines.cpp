// TODO: impl thread::close.
// TODO: use fiber ex functions to preserve floats.
// TODO: you can create fiber only from another fiber.

#include "Coroutines.hpp"

#include "Ensures.hpp"
#include "Intrin.hpp"
#include "ManageObject.hpp"
#include "Memory.hpp"
#include "NonCopyable.hpp"
#include "Queue.hpp"
#include "SpinLock.hpp"
#include "Templates.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines;


namespace
{
    class UserFiber final : public NonCopyable
    {
    public:
        enum InitPending { kPending };
        enum InitSpawned { kSpawned };

        using Action_t = void(*)(uintptr_t const userData);

        UserFiber();
        UserFiber(InitPending const, Action_t const fiberAction, uintptr_t const userData);
        UserFiber(InitSpawned const, void *const fiberHandle);
        ~UserFiber();

        void* GetOrAddHandle();

    private:
        enum class State { kInvalid, kPending, kSpawned };

        struct Pending final { Action_t m_fiberAction; uintptr_t m_userData; };
        struct Spawned final { void* m_fiberHandle; };

        State m_state;
        union { Pending m_pending; Spawned m_spawned; };
    };


    class ThreadSharedData final : public NonCopyable
    {
    public:
        static void Init();
        static void Shut();

        static uint32_t GetTlsHandle();

        static void EnqueueUserFiber(UserFiber *const userFiber);
        static UserFiber* DequeueUserFiber();

    private:
        static uint32_t GTlsHandle;

        static SpinLock GUserFibersLock;
        static ManageObject<Queue<UserFiber*>> GUserFibers;
    };


    class Thread final : public NonCopyable
    {
    public:
        static void Yield();
        [[noreturn]] static void Close();

        Thread();
        ~Thread();

    private:
        static uint32_t __stdcall ThreadAction(void *const userData);
        void ThreadAction();

        SpinLock m_initLock;
        void* m_threadHandle;
        volatile uintptr_t m_threadStop;
        void* m_fiberHandle;
    };


    class ThreadArray final : public NonCopyable
    {
    public:
        static void Init();
        static void Shut();

    private:
        static ManageObject<Thread> GThread;
    };
}
// namespace


uint32_t ThreadSharedData::GTlsHandle;
SpinLock ThreadSharedData::GUserFibersLock;
ManageObject<Queue<UserFiber*>> ThreadSharedData::GUserFibers;
ManageObject<Thread> ThreadArray::GThread;


UserFiber::UserFiber()
{
    m_state = State::kInvalid;
}


UserFiber::UserFiber(InitPending const, Action_t const fiberAction, uintptr_t const userData)
{
    if (!fiberAction)
        CheckNoEntry();

    m_state = State::kPending;
    m_pending = { fiberAction, userData };
}


UserFiber::UserFiber(InitSpawned const, void *const fiberHandle)
{
    if (!fiberHandle)
        CheckNoEntry();

    m_state = State::kSpawned;
    m_spawned = { fiberHandle };
}


UserFiber::~UserFiber()
{
    if (m_state != State::kSpawned)
        return;

    WinApi::DeleteFiber(m_spawned.m_fiberHandle);
}


void* UserFiber::GetOrAddHandle()
{
    if (m_state == State::kSpawned)
        return m_spawned.m_fiberHandle;

    if (m_state != State::kPending)
        CheckNoEntry();

    m_state = State::kSpawned;
    m_spawned = { WinApi::CreateFiber(16384, (void*)m_pending.m_fiberAction, (void*)m_pending.m_userData) };

    if (!m_spawned.m_fiberHandle)
        CheckNoEntry();

    return m_spawned.m_fiberHandle;
}


void ThreadSharedData::Init()
{
    GTlsHandle = WinApi::TlsAlloc();
    if (GTlsHandle == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    GUserFibers.Construct();
}


void ThreadSharedData::Shut()
{
    GUserFibers.Destruct();

    if (!WinApi::TlsFree(GTlsHandle))
        CheckNoEntry();
    GTlsHandle = WinApi::TLS_OUT_OF_INDEXES;
}


uint32_t ThreadSharedData::GetTlsHandle()
{
    return GTlsHandle;
}


void ThreadSharedData::EnqueueUserFiber(UserFiber *const userFiber)
{
    if (!userFiber)
        CheckNoEntry();

    SpinLockScope_t const lockScope{ GUserFibersLock };
    GUserFibers->Enqueue(userFiber);
}


UserFiber* ThreadSharedData::DequeueUserFiber()
{
    SpinLockScope_t const lockScope{ GUserFibersLock };
    if (!GUserFibers->IsEmpty())
        return GUserFibers->Dequeue();
    else
        return {};
}


void Thread::Yield()
{
    auto const self = (Thread*)WinApi::TlsGetValue(ThreadSharedData::GetTlsHandle());
    WinApi::SwitchToFiber(self->m_fiberHandle);
}


void Thread::Close()
{
    auto const self = (Thread*)WinApi::TlsGetValue(ThreadSharedData::GetTlsHandle());
    WinApi::SwitchToFiber(self->m_fiberHandle);
}


Thread::Thread()
{
    Intrin::AtomicExchange(&m_threadStop, 0);
    m_initLock.Lock();

    m_threadHandle = WinApi::CreateThread({}, 16384, ThreadAction, this, {}, {});
    if (!m_threadHandle)
        CheckNoEntry();
}


Thread::~Thread()
{
    Intrin::AtomicExchange(&m_threadStop, 1);
    m_initLock.Lock();
}


uint32_t __stdcall Thread::ThreadAction(void *const userData)
{
    auto const self = (Thread*)userData;
    self->ThreadAction();

    return {};
}


void Thread::ThreadAction()
{
    if (!WinApi::TlsSetValue(ThreadSharedData::GetTlsHandle(), this))
        CheckNoEntry();

    m_fiberHandle = WinApi::ConvertThreadToFiber({});
    if (!m_fiberHandle)
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&m_threadStop, 0, 0) == 0)
    {
        auto const userFiber = ThreadSharedData::DequeueUserFiber();
        if (!userFiber)
        {
            Intrin::YieldProcessor();
            continue;
        }

        auto const fiberHandle = userFiber->GetOrAddHandle();
        if (!fiberHandle)
            CheckNoEntry();

        WinApi::SwitchToFiber(fiberHandle);
        ThreadSharedData::EnqueueUserFiber(userFiber);

        Intrin::YieldProcessor();
    }

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();
    m_fiberHandle = {};

    if (!WinApi::TlsSetValue(ThreadSharedData::GetTlsHandle(), {}))
        CheckNoEntry();

    m_initLock.Unlock();
}


void ThreadArray::Init()
{
    GThread.Construct();
}


void ThreadArray::Shut()
{
    GThread.Destruct();
}


void Coroutines::InitSystem()
{
    ThreadSharedData::Init();
    ThreadArray::Init();
}


void Coroutines::ShutSystem()
{
    ThreadArray::Shut();
    ThreadSharedData::Shut();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    auto const userFiber = &Memory::GetHeapAlloc().Create<UserFiber>(UserFiber::kPending, coroutineAction, userData);
    ThreadSharedData::EnqueueUserFiber(userFiber);
}


void Coroutines::Yield()
{
    Thread::Yield();
}


[[noreturn]] void Coroutines::Close()
{
    Thread::Close();
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


//     struct Thread final
//     {
//         volatile uintptr_t thread{};
//         volatile uintptr_t fiber{};

//         volatile FiberState userState{};
//     };


//     struct System final
//     {
//         uint32_t tlsHandle{ WinApi::TLS_OUT_OF_INDEXES };
//         Span<Thread> contexts{};

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


// static void InitThreads()
// {
//     GSystem->contexts = Memory::GetHeapAlloc().CreateArray<Thread>(GetNumberOfCores());
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


// static void ShutThreads()
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
//     InitThreads();
// }


// void Coroutines::ShutSystem()
// {
//     if (!GSystem)
//         CheckNoEntry();

//     Intrin::AtomicExchange(&GSystem->exitFlag, uintptr_t{ 1 });
//     ShutThreads();
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
