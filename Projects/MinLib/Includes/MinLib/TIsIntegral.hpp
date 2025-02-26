#pragma once
#include "TBoolConstant.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    struct TIsIntegral : public TBoolConstant<(T)3/(T)2 == (T)1> {};
}
