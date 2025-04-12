#pragma once


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
        volatile bool m_locked{};
    };
};
// namespace Aiva
