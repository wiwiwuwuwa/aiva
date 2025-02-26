#pragma once
#include "EArithmeticType.hpp"
#include "TIntegralConstant.hpp"
#include "TIsFloating.hpp"
#include "TIsIntegral.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    constexpr auto TGetArithmeticTypeF()
    {
        if constexpr (TIsIntegral<T>::Value)
            { return EArithmeticType::Integral; }

        if constexpr (TIsFloating<T>::Value)
            { return EArithmeticType::Floating; }
    }

    template <typename T>
    struct TGetArithmeticType : public TIntegralConstant<EArithmeticType, TGetArithmeticTypeF<T>()> {};
}
// namespace Aiva::MinLib
