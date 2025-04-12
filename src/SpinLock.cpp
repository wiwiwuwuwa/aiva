#include "SpinLock.hpp"
#include "Intrin.hpp"


using namespace Aiva;


void SpinLock::Lock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, false, true) == false)
            break;

        Intrin::YieldProcessor();
    }
}


void SpinLock::Unlock()
{
    while (true)
    {
        if (Intrin::AtomicCompareExchange(&m_locked, true, false) == true)
            break;

        Intrin::YieldProcessor();
    }
}
