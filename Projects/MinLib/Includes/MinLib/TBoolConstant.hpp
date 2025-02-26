#pragma once
#include "TIntegralConstant.hpp"

namespace Aiva::MinLib
{
    template <bool V>
    struct TBoolConstant : public TIntegralConstant<bool, V> {};
}
