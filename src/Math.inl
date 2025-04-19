#pragma once
#include "Math.hpp"


namespace Aiva::Math
{
    template <typename TType>
    constexpr TType Abs(TType const value)
    {
        return value >= TType{} ? value : -value;
    }


    template <typename TType>
    constexpr TType Min(TType const a, TType const b)
    {
        return a <= b ? a : b;
    }
}
// namespace Aiva::Math
