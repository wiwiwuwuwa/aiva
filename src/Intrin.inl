#pragma once
#include "Intrin.hpp"

#include "Templates.hpp"


namespace Aiva::Intrin
{
    template <typename TType>
    inline TType AtomicCompareExchange(volatile TType *const Destination, TType const Comperand, TType const Exchange)
    {
        using namespace Aiva::Templates;
        using NumberIdentity_t = NumberIdentity_t<NumberType::Int, NumberSign::None, GetNumberSize<TType>()>;

        auto volatile *const destination = (volatile NumberIdentity_t*)(volatile void*)Destination;
        auto comperand = *(NumberIdentity_t*)(void*)&Comperand;
        auto exchange = *(NumberIdentity_t*)(void*)&Exchange;
        __atomic_compare_exchange(destination, &comperand, &exchange, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

        return *(TType*)(void*)&comperand;
    }


    template <typename TType>
    inline TType AtomicExchange(volatile TType *const Destination, TType const Exchange)
    {
        using namespace Aiva::Templates;
        using NumberIdentity_t = NumberIdentity_t<NumberType::Int, NumberSign::None, GetNumberSize<TType>()>;

        auto volatile *const destination = (volatile NumberIdentity_t*)(volatile void*)Destination;
        auto exchange = *(NumberIdentity_t*)(void*)&Exchange;
        auto previous = NumberIdentity_t{};
        __atomic_exchange(destination, &exchange, &previous, __ATOMIC_SEQ_CST);

        return *(TType*)(void*)&previous;
    }


    inline void YieldProcessor()
    {
        __builtin_ia32_pause();
    }
}
// namespace Aiva::Intrin
