#pragma once
#include "TToArithmeticSign.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    struct TGetArithmeticSign : public TToArithmeticSign<(T)-1 < (T)0> {};
}
// namespace Aiva::MinLib
