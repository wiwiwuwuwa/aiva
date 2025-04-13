#include "SpinLock.hpp"

#include "Intrin.hpp"


using namespace Aiva;


void SpinLock::Lock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, 0, 1) == 0)
            break;

        Intrin::YieldProcessor();
    }
}


void SpinLock::Unlock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, 1, 0) == 1)
            break;

        Intrin::YieldProcessor();
    }
}
