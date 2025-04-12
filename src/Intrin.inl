#pragma once
#include "Intrin.hpp"


namespace Aiva::Intrin
{
    inline uintptr_t AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange)
    {
        auto comperand = Comperand;
        auto exchange = Exchange;
        __atomic_compare_exchange(Destination, &comperand, &exchange, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

        return comperand;
    }


    inline uintptr_t AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange)
    {
        auto exchange = Exchange;
        auto previous = uintptr_t{};
        __atomic_exchange(Destination, &exchange, &previous, __ATOMIC_SEQ_CST);

        return previous;
    }


    inline void YieldProcessor()
    {
        __builtin_ia32_pause();
    }
}
// namespace Aiva::Intrin
