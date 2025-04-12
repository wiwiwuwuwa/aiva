#pragma once


namespace Aiva
{
    template <typename TType>
    class LockScope
    {
    public:
        LockScope(TType& lock);
        ~LockScope();

        LockScope(const LockScope&) = delete;
        LockScope& operator=(const LockScope&) = delete;
        LockScope(LockScope&&) = delete;
        LockScope& operator=(LockScope&&) = delete;

    private:
        TType& m_lock;
    };
}
// namespace Aiva


#include "LockScope.inl"
