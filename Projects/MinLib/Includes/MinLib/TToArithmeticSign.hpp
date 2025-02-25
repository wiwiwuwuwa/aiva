#pragma once
#include "EArithmeticSign.hpp"
#include "TIntegralConstant.hpp"

namespace Aiva::MinLib
{
    template <bool V>
    struct TToArithmeticSign;

    template <>
    struct TToArithmeticSign<true> : public TIntegralConstant<EArithmeticSign, EArithmeticSign::Signed> {};

    template <>
    struct TToArithmeticSign<false> : public TIntegralConstant<EArithmeticSign, EArithmeticSign::Unsigned> {};
}
// namespace Aiva::MinLib
