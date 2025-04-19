#pragma once


namespace Aiva::Math
{
    template <typename TType>
    constexpr TType Abs(TType const value);


    template <typename TType>
    constexpr TType Min(TType const a, TType const b);
}
// namespace Aiva::Math


#include "Math.inl"
