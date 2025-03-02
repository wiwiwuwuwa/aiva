#pragma once
#include <MinLib/ArithmeticType.hpp>
#include <MinLib/IntegralConstant.hpp>
#include <MinLib/IsFloating.hpp>
#include <MinLib/IsIntegral.hpp>

namespace Aiva::MinLib
{
    template <typename T>
    constexpr auto GetArithmeticType_Impl()
    {
        if constexpr (IsIntegral<T>::Value)
            { return ArithmeticType::Integral; }

        if constexpr (IsFloating<T>::Value)
            { return ArithmeticType::Floating; }
    }

    template <typename T>
    struct GetArithmeticType : public IntegralConstant<ArithmeticType, GetArithmeticType_Impl<T>()> {};
}
// namespace Aiva::MinLib
