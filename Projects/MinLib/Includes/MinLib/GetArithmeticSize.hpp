#pragma once
#include <MinLib/ToArithmeticSize.hpp>

namespace Aiva::MinLib
{
    template <typename T>
    struct GetArithmeticSize : public ToArithmeticSize<sizeof(T)> {};
}
// namespace Aiva::MinLib
