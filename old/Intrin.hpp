#pragma once

#include "Numbers.hpp"


namespace Aiva::Intrin
{
    inline uintptr_t AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange);
    inline uintptr_t AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange);
    inline void YieldProcessor();
}
// namespace Aiva::Intrin


#include "Intrin.inl"
