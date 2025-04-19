#include "Coroutines.hpp"

#include "Console.hpp"
#include "CstrView.hpp"
#include "Ensures.hpp"
#include "Intrin.hpp"
#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "Process.hpp"
#include "Queue.hpp"
#include "Span.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines;


namespace
{
    class UserFiber final : public NonCopyable
    {
    public:
        using Action_t = void(&)(uintptr_t const userData);

        UserFiber(Action_t fiberAction, uintptr_t const userData);
        ~UserFiber();

        void SwitchToFiber();

    private:
        __attribute__((stdcall)) static void FiberAction(void *const userData);

        Action_t m_fiberAction;
        uintptr_t m_userData;

        void* m_fiberHandle;
    };


    class FiberQueue final : public NonCopyable
    {
    public:
        void Enqueue(UserFiber& userFiber);
        UserFiber* Dequeue();

    private:
        SpinLock m_lock;
        Queue<UserFiber*> m_queue;
    };


    class Thread final : public NonCopyable
    {
    public:
        Thread(uint32_t const threadLocalStorage, FiberQueue& fiberQueue);
        ~Thread();

        void SwitchToFiber(bool const closeUserFiber);

    private:
        __attribute__((stdcall)) static uint32_t ThreadAction(void *const userData);
        void ThreadAction();

        uint32_t m_threadLocalStorage;
        FiberQueue& m_fiberQueue;
        volatile uintptr_t m_stopFlag;
        void* m_threadHandle;
        void* m_fiberHandle;
        bool m_closeUserFiber;
    };


    class System final : public NonCopyable
    {
    public:
        System();
        ~System();

        uint32_t GetThreadLocalStorage() const;
        FiberQueue& GetFiberQueue();

    private:
        uint32_t m_threadLocalStorage;
        ManageObject<FiberQueue> m_fiberQueue;
        Span<Thread> m_threads;
    };


    SpinLock GSystemLock;
    ManageObject<System> GSystem;


    size_t GetNumberOfCores()
    {
        auto systemInfo = WinApi::SYSTEM_INFO{};
        WinApi::GetSystemInfo(&systemInfo);

        return (size_t)systemInfo.dwNumberOfProcessors;
    }
}
// namespace


UserFiber::UserFiber(Action_t fiberAction, uintptr_t const userData) :
    m_fiberAction{ fiberAction }, m_userData{ userData }, m_fiberHandle{ nullptr }
{
    if (!fiberAction)
        CheckNoEntry();
}


UserFiber::~UserFiber()
{
    if (!m_fiberHandle)
        return;

    WinApi::DeleteFiber(m_fiberHandle);
    m_fiberHandle = nullptr;
}


void UserFiber::SwitchToFiber()
{
    if (!m_fiberHandle)
        m_fiberHandle = WinApi::CreateFiber(16384, FiberAction, this);

    if (!m_fiberHandle)
        CheckNoEntry();

    WinApi::SwitchToFiber(m_fiberHandle);
}


__attribute__((stdcall)) void UserFiber::FiberAction(void *const userData)
{
    auto const self = (UserFiber*)userData;
    self->m_fiberAction(self->m_userData);
}


void FiberQueue::Enqueue(UserFiber& userFiber)
{
    SpinLockScope_t const lockScope{ m_lock };
    m_queue.Enqueue(&userFiber);
}


UserFiber* FiberQueue::Dequeue()
{
    SpinLockScope_t const lockScope{ m_lock };

    if (!m_queue.IsEmpty())
        return m_queue.Dequeue();
    else
        return nullptr;
}


Thread::Thread(uint32_t const threadLocalStorage, FiberQueue& fiberQueue) :
    m_threadLocalStorage{ threadLocalStorage}, m_fiberQueue{ fiberQueue }, m_stopFlag{ 0 }
{
    if (threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    m_threadHandle = WinApi::CreateThread(nullptr, 16384, ThreadAction, this, 0, nullptr);
    if (!m_threadHandle)
        CheckNoEntry();
}


Thread::~Thread()
{
    Intrin::AtomicExchange(&m_stopFlag, 1);

    if (WinApi::WaitForSingleObject(m_threadHandle, WinApi::INFINITE) == WinApi::WAIT_FAILED)
        CheckNoEntry();

    if (!WinApi::CloseHandle(m_threadHandle))
        CheckNoEntry();
}


void Thread::SwitchToFiber(bool const closeUserFiber)
{
    if (!m_fiberHandle)
        CheckNoEntry();

    m_closeUserFiber = closeUserFiber;
    WinApi::SwitchToFiber(m_fiberHandle);
}


__attribute__((stdcall)) uint32_t Thread::ThreadAction(void *const userData)
{
    auto const self = (Thread*)userData;
    self->ThreadAction();

    return 0;
}


void Thread::ThreadAction()
{
    if (!WinApi::TlsSetValue(m_threadLocalStorage, this))
        CheckNoEntry();

    m_fiberHandle = WinApi::ConvertThreadToFiber(nullptr);
    if (!m_fiberHandle)
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&m_stopFlag, 0, 0) == 0)
    {
        auto const userFiber = m_fiberQueue.Dequeue();
        if (!userFiber)
        {
            Intrin::YieldProcessor();
            continue;
        }

        m_closeUserFiber = false;
        userFiber->SwitchToFiber();

        auto const closeUserFiber = m_closeUserFiber;
        m_closeUserFiber = false;

        if (closeUserFiber)
        {
            Memory::GetHeapAlloc().Delete(*userFiber);
            continue;
        }

        m_fiberQueue.Enqueue(*userFiber);
    }

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();
    m_fiberHandle = nullptr;

    if (!WinApi::TlsSetValue(m_threadLocalStorage, nullptr))
        CheckNoEntry();
}


System::System()
{
    m_threadLocalStorage = WinApi::TlsAlloc();
    if (m_threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    m_fiberQueue.Construct();

    m_threads = Memory::GetHeapAlloc().CreateArray<Thread>(GetNumberOfCores(), m_threadLocalStorage, m_fiberQueue.GetObject());
    if (!m_threads)
        CheckNoEntry();
}


System::~System()
{
    m_threads = Memory::GetHeapAlloc().DeleteArray(m_threads);
    if (m_threads)
        CheckNoEntry();

    m_fiberQueue.Destruct();

    if (!WinApi::TlsFree(m_threadLocalStorage))
        CheckNoEntry();
    m_threadLocalStorage = WinApi::TLS_OUT_OF_INDEXES;
}


uint32_t System::GetThreadLocalStorage() const
{
    return m_threadLocalStorage;
}


FiberQueue& System::GetFiberQueue()
{
    return m_fiberQueue.GetObject();
}


void Coroutines::InitSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (GSystem)
        CheckNoEntry();

    GSystem.Construct();
}


void Coroutines::ShutSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    GSystem.Destruct();
}


void Coroutines::Spawn(CoroutineAction_t coroutineAction, uintptr_t const userData)
{
    auto& userFiber = Memory::GetHeapAlloc().Create<UserFiber>(coroutineAction, userData);

    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    GSystem->GetFiberQueue().Enqueue(userFiber);
}


void Coroutines::Yield()
{
    auto threadLocalStorage = uint32_t{};

    {
        SpinLockScope_t const lockScope{ GSystemLock };

        if (!GSystem)
            CheckNoEntry();

        threadLocalStorage = GSystem->GetThreadLocalStorage();
    }

    if (threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    auto thread = (Thread*)WinApi::TlsGetValue(threadLocalStorage);
    if (!thread)
        CheckNoEntry();

    thread->SwitchToFiber(false);
}


[[noreturn]] void Coroutines::Close()
{
    auto threadLocalStorage = uint32_t{};

    {
        SpinLockScope_t const lockScope{ GSystemLock };

        if (!GSystem)
            CheckNoEntry();

        threadLocalStorage = GSystem->GetThreadLocalStorage();
    }

    if (threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    auto thread = (Thread*)WinApi::TlsGetValue(threadLocalStorage);
    if (!thread)
        CheckNoEntry();

    thread->SwitchToFiber(true);

    CheckNoEntry();
}
