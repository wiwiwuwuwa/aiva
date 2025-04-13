#include "SpinLock.hpp"

#include "Intrin.hpp"


using namespace Aiva;


void SpinLock::Lock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, uintptr_t{ 0 }, uintptr_t{ 1 }) == uintptr_t{ 0 })
            break;

        Intrin::YieldProcessor();
    }
}


void SpinLock::Unlock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, uintptr_t{ 1 }, uintptr_t{ 0 }) == uintptr_t{ 1 })
            break;

        Intrin::YieldProcessor();
    }
}
