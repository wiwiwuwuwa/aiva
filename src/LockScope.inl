#pragma once

#include "LockScope.hpp"


namespace Aiva
{
    template <typename TType>
    LockScope<TType>::LockScope(TType const& lock) : m_lock{ const_cast<TType&>(lock) }
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
