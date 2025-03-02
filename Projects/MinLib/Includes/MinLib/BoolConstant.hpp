#pragma once
#include <MinLib/IntegralConstant.hpp>

namespace Aiva::MinLib
{
    template <bool V>
    struct BoolConstant : public IntegralConstant<bool, V> {};
}
// namespace Aiva::MinLib
