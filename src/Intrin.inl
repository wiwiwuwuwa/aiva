#pragma once
#include "Intrin.hpp"


using namespace Aiva;
using namespace Aiva::Intrin;


uintptr_t Intrin::AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange)
{
    auto comperand = Comperand;
    auto exchange = Exchange;
    __atomic_compare_exchange(Destination, &comperand, &exchange, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

    return comperand;
}


uintptr_t Intrin::AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange)
{
    auto exchange = Exchange;
    auto previous = uintptr_t{};
    __atomic_exchange(Destination, &exchange, &previous, __ATOMIC_SEQ_CST);

    return previous;
}


void Intrin::YieldProcessor()
{
     __builtin_ia32_pause();
}
