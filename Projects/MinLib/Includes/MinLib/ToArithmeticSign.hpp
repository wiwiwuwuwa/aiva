#pragma once
#include <MinLib/ArithmeticSign.hpp>
#include <MinLib/IntegralConstant.hpp>

namespace Aiva::MinLib
{
    template <bool V>
    struct ToArithmeticSign;

    template <>
    struct ToArithmeticSign<true> : public IntegralConstant<ArithmeticSign, ArithmeticSign::Signed> {};

    template <>
    struct ToArithmeticSign<false> : public IntegralConstant<ArithmeticSign, ArithmeticSign::Unsigned> {};
}
// namespace Aiva::MinLib
