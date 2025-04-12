#pragma once
#include "Numbers.hpp"


namespace Aiva::Intrin
{
    uintptr_t AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange);
    uintptr_t AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange);
    void YieldProcessor();
}
// namespace Aiva::Intrin


#include "Intrin.inl"
