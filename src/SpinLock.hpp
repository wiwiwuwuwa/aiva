#pragma once
#include "LockScope.hpp"
#include "NonCopyable.hpp"
#include "Numbers.hpp"


namespace Aiva
{
    class SpinLock final : public NonCopyable
    {
    public:
        SpinLock() = default;
        ~SpinLock() = default;

        void Lock();
        void Unlock();

    private:
        volatile uintptr_t m_locked{};
    };


    using SpinLockScope_t = LockScope<SpinLock>;
};
// namespace Aiva
