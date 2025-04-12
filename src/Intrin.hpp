#pragma once
#include "Numbers.hpp"


namespace Aiva::Intrin
{
    template <typename TType>
    inline TType AtomicCompareExchange(volatile TType *const Destination, TType const Comperand, TType const Exchange);

    template <typename TType>
    inline TType AtomicExchange(volatile TType *const Destination, TType const Exchange);

    inline void YieldProcessor();
}
// namespace Aiva::Intrin


#include "Intrin.inl"
