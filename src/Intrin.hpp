#pragma once
#include "Numbers.hpp"


namespace Aiva::Intrin
{
    template <typename TType>
    TType AtomicCompareExchange(volatile TType *const Destination, TType const Comperand, TType const Exchange);

    template <typename TType>
    TType AtomicExchange(volatile TType *const Destination, TType const Exchange);

    void YieldProcessor();
}
// namespace Aiva::Intrin


#include "Intrin.inl"
