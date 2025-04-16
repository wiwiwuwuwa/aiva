#pragma once
#include "Math.hpp"


namespace Aiva::Math
{
    template <typename TType>
    constexpr TType Abs(TType const value)
    {
        return value >= TType{} ? value : -value;
    }
}
// namespace Aiva::Math
