#pragma once
#include <MinLib/ToArithmeticSign.hpp>

namespace Aiva::MinLib
{
    template <typename T>
    struct GetArithmeticSign : public ToArithmeticSign<(T)-1 < (T)0> {};
}
// namespace Aiva::MinLib
