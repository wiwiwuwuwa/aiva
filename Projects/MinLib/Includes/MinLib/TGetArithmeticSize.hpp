#pragma once
#include "TToArithmeticSize.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    struct TGetArithmeticSize : public TToArithmeticSize<sizeof(T)> {};
}
// namespace Aiva::MinLib
