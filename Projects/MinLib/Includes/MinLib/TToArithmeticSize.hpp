#pragma once
#include "EArithmeticSize.hpp"
#include "TIntegralConstant.hpp"

namespace Aiva::MinLib
{
    template <decltype(sizeof(unsigned char)) V>
    struct TToArithmeticSize;

    template <>
    struct TToArithmeticSize<1> : public TIntegralConstant<EArithmeticSize, EArithmeticSize::Byte1> {};

    template <>
    struct TToArithmeticSize<2> : public TIntegralConstant<EArithmeticSize, EArithmeticSize::Byte2> {};

    template <>
    struct TToArithmeticSize<4> : public TIntegralConstant<EArithmeticSize, EArithmeticSize::Byte4> {};

    template <>
    struct TToArithmeticSize<8> : public TIntegralConstant<EArithmeticSize, EArithmeticSize::Byte8> {};
}
// namespace Aiva::MinLib
