#pragma once
#include "LockScope.hpp"
#include "Numbers.hpp"


namespace Aiva
{
    class SpinLock final
    {
    public:
        SpinLock() = default;
        ~SpinLock() = default;

        SpinLock(const SpinLock&) = delete;
        SpinLock& operator=(const SpinLock&) = delete;
        SpinLock(SpinLock&&) = delete;
        SpinLock& operator=(SpinLock&&) = delete;

        void Lock();
        void Unlock();

    private:
        volatile uintptr_t m_locked{};
    };


    using SpinLockScope_t = LockScope<SpinLock>;
};
// namespace Aiva
