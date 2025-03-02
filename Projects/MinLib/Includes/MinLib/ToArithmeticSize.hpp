#pragma once
#include <MinLib/ArithmeticSize.hpp>
#include <MinLib/IntegralConstant.hpp>

namespace Aiva::MinLib
{
    template <decltype(sizeof(unsigned char)) V>
    struct ToArithmeticSize;

    template <>
    struct ToArithmeticSize<1> : public IntegralConstant<ArithmeticSize, ArithmeticSize::Byte1> {};

    template <>
    struct ToArithmeticSize<2> : public IntegralConstant<ArithmeticSize, ArithmeticSize::Byte2> {};

    template <>
    struct ToArithmeticSize<4> : public IntegralConstant<ArithmeticSize, ArithmeticSize::Byte4> {};

    template <>
    struct ToArithmeticSize<8> : public IntegralConstant<ArithmeticSize, ArithmeticSize::Byte8> {};
}
// namespace Aiva::MinLib
