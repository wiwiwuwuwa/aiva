#pragma once
#include "LockScope.hpp"


namespace Aiva
{
    template <typename TType>
    LockScope<TType>::LockScope(TType& lock) : m_lock{ lock }
    {
        m_lock.Lock();
    }


    template <typename TType>
    LockScope<TType>::~LockScope()
    {
        m_lock.Unlock();
    }
}
// namespace Aiva
