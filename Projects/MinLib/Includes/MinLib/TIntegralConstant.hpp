#pragma once

namespace Aiva::MinLib
{
    template<typename T, T value>
    struct TIntegralConstant
    {
        static constexpr const T Value = value;
    };
}
// namespace Aiva::MinLib
