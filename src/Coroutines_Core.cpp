#include "Coroutines_Core.hpp"

#include "Console.hpp"
#include "CstrView.hpp"
#include "Ensures.hpp"
#include "Intrin.hpp"
#include "LinkedList.hpp"
#include "ManageObject.hpp"
#include "Math.hpp"
#include "NonCopyable.hpp"
#include "Process.hpp"
#include "Span.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Coroutines::Core;


namespace
{
    class UserFiber final : public NonCopyable
    {
    public:
        UserFiber(CoroutineFunc_t coroutineFunc, uintptr_t const userData, uintptr_t const workerMask);
        ~UserFiber();

        uintptr_t GetUserData() const;
        uintptr_t GetWorkerMask() const;
        void SetWorkerMask(uintptr_t const workerMask);
        void* GetOrAddFiberHandle();

    private:
        __attribute__((stdcall)) static void FiberFunc(void *const userData);

        CoroutineFunc_t m_coroutineFunc;
        uintptr_t m_userData;
        uintptr_t m_workerMask;
        void* m_fiberHandle;
    };


    class FiberQueue final : public NonCopyable
    {
    public:
        void PushBack(UserFiber& userFiber);
        UserFiber* PopFirst(uintptr_t const workerMask);

    private:
        SpinLock m_lock;
        LinkedList<UserFiber*> m_queue;
    };


    class Thread final : public NonCopyable
    {
    public:
        Thread(uintptr_t const workerMask, uint32_t const threadLocalStorage, FiberQueue& fiberQueue);
        ~Thread();

        static Thread const& GetCurrentThread();
        uintptr_t GetWorkerMask() const;
        void* GetFiberHandle() const;
        UserFiber& GetUserFiber() const;

    private:
        __attribute__((stdcall)) static uint32_t ThreadFunc(void *const userData);

        uintptr_t m_workerMask;
        uint32_t m_threadLocalStorage;
        FiberQueue& m_fiberQueue;
        volatile uintptr_t m_stopFlag;
        void* m_threadHandle;
        void* m_fiberHandle;
        UserFiber* m_userFiber;
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

        return Math::Min(Templates::CountOfBits_v<uintptr_t>, (size_t)systemInfo.dwNumberOfProcessors);
    }


    uintptr_t Control_GetUserData()
    {
        return Thread::GetCurrentThread().GetUserFiber().GetUserData();
    }


    uintptr_t Control_GetCurrWorker()
    {
        return Thread::GetCurrentThread().GetWorkerMask();
    }


    void Control_Yield()
    {
        Thread::GetCurrentThread().GetUserFiber().SetWorkerMask(kAnyWorkerMask);
        WinApi::SwitchToFiber(Thread::GetCurrentThread().GetFiberHandle());
    }


    void Control_YieldOnWorker(uintptr_t const workerMask)
    {
        Thread::GetCurrentThread().GetUserFiber().SetWorkerMask(workerMask);
        WinApi::SwitchToFiber(Thread::GetCurrentThread().GetFiberHandle());
    }


    void Control_CloseFunc()
    {
        Thread::GetCurrentThread().GetUserFiber().SetWorkerMask(0);
        WinApi::SwitchToFiber(Thread::GetCurrentThread().GetFiberHandle());
    }
}
// namespace


UserFiber::UserFiber(CoroutineFunc_t coroutineFunc, uintptr_t const userData, uintptr_t const workerMask)
    : m_coroutineFunc{ coroutineFunc }, m_userData{ userData }, m_workerMask{ workerMask }, m_fiberHandle{ nullptr }
{
    if (!coroutineFunc)
        CheckNoEntry();
}


UserFiber::~UserFiber()
{
    if (m_fiberHandle)
        WinApi::DeleteFiber(m_fiberHandle);
}


uintptr_t UserFiber::GetUserData() const
{
    return m_userData;
}


uintptr_t UserFiber::GetWorkerMask() const
{
    return m_workerMask;
}


void UserFiber::SetWorkerMask(uintptr_t const workerMask)
{
    m_workerMask = workerMask;
}


void* UserFiber::GetOrAddFiberHandle()
{
    if (m_fiberHandle)
        return m_fiberHandle;

    m_fiberHandle = WinApi::CreateFiber(16384, FiberFunc, this);

    if (!m_fiberHandle)
        CheckNoEntry();

    return m_fiberHandle;
}


__attribute__((stdcall)) void UserFiber::FiberFunc(void *const userData)
{
    auto const self = (UserFiber*)userData;
    if (!self)
        CheckNoEntry();

    auto const control = Control
    {
        Control_GetUserData,
        Control_GetCurrWorker,
        Control_Yield,
        Control_YieldOnWorker
    };
    self->m_coroutineFunc(control);

    Control_CloseFunc();
    CheckNoEntry();
}


void FiberQueue::PushBack(UserFiber& userFiber)
{
    SpinLockScope_t const lockScope{ m_lock };
    m_queue.PushBack(&userFiber);
}


UserFiber* FiberQueue::PopFirst(uintptr_t const workerMask)
{
    SpinLockScope_t const lockScope{ m_lock };
    return m_queue.PopFirst([&](auto const userFiber) { return (userFiber->GetWorkerMask() & workerMask) != 0; });
}


Thread::Thread(uintptr_t const workerMask, uint32_t const threadLocalStorage, FiberQueue& fiberQueue) :
    m_workerMask{ workerMask }, m_threadLocalStorage{ threadLocalStorage}, m_fiberQueue{ fiberQueue }, m_stopFlag{ 0 }
{
    if (threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    m_threadHandle = WinApi::CreateThread(nullptr, 16384, ThreadFunc, this, 0, nullptr);
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


Thread const& Thread::GetCurrentThread()
{
    auto const threadLocalStorage = GSystem->GetThreadLocalStorage();
    if (threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    auto const thread = (Thread*)WinApi::TlsGetValue(threadLocalStorage);
    if (!thread)
        CheckNoEntry();

    return *thread;
}


uintptr_t Thread::GetWorkerMask() const
{
    return m_workerMask;
}


void* Thread::GetFiberHandle() const
{
    if (!m_fiberHandle)
        CheckNoEntry();

    return m_fiberHandle;
}


UserFiber& Thread::GetUserFiber() const
{
    if (!m_userFiber)
        CheckNoEntry();

    return *m_userFiber;
}


__attribute__((stdcall)) uint32_t Thread::ThreadFunc(void *const userData)
{
    auto const self = (Thread*)userData;
    if (!self)
        CheckNoEntry();

    if (!WinApi::TlsSetValue(self->m_threadLocalStorage, self))
        CheckNoEntry();

    self->m_fiberHandle = WinApi::ConvertThreadToFiber(nullptr);
    if (!self->m_fiberHandle)
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&self->m_stopFlag, 0, 0) == 0)
    {
        auto const userFiber = self->m_fiberQueue.PopFirst(self->m_workerMask);
        if (!userFiber)
        {
            Intrin::YieldProcessor();
            continue;
        }

        auto const fiberHandle = userFiber->GetOrAddFiberHandle();
        if (!fiberHandle)
            CheckNoEntry();

        self->m_userFiber = userFiber;
        WinApi::SwitchToFiber(fiberHandle);
        self->m_userFiber = nullptr;

        if (!userFiber->GetWorkerMask())
        {
            Memory::GetHeapAlloc().Delete(*userFiber);
            continue;
        }

        self->m_fiberQueue.PushBack(*userFiber);
    }

    if (!WinApi::ConvertFiberToThread())
        CheckNoEntry();
    self->m_fiberHandle = nullptr;

    if (!WinApi::TlsSetValue(self->m_threadLocalStorage, nullptr))
        CheckNoEntry();

    return 0;
}


System::System()
{
    m_threadLocalStorage = WinApi::TlsAlloc();
    if (m_threadLocalStorage == WinApi::TLS_OUT_OF_INDEXES)
        CheckNoEntry();

    m_fiberQueue.Construct();

    m_threads = Memory::GetHeapAlloc().AllocArray<Thread>(GetNumberOfCores());
    if (!m_threads)
        CheckNoEntry();

    for (auto i = size_t{}; i < m_threads.GetSize(); ++i)
        new (&m_threads[i]) Thread(1 << i, m_threadLocalStorage, m_fiberQueue.GetObjectRef());
}


System::~System()
{
    for (auto i = m_threads.GetSize(); i > 0; i--)
        m_threads[i - 1].~Thread();

    m_threads = Memory::GetHeapAlloc().FreeArray(m_threads);
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
    return m_fiberQueue.GetObjectRef();
}


void Coroutines::Core::InitSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (GSystem)
        CheckNoEntry();

    GSystem.Construct();
}


void Coroutines::Core::ShutSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    GSystem.Destruct();
}


void Coroutines::Core::Spawn(CoroutineFunc_t coroutineFunc, uintptr_t const userData)
{
    SpawnOnWorker(coroutineFunc, kAnyWorkerMask, userData);
}


void Coroutines::Core::SpawnOnWorker(CoroutineFunc_t coroutineFunc, uintptr_t const workerMask, uintptr_t const userData)
{
    auto& userFiber = Memory::GetHeapAlloc().Create<UserFiber>(coroutineFunc, userData, workerMask);

    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    GSystem->GetFiberQueue().PushBack(userFiber);
}
