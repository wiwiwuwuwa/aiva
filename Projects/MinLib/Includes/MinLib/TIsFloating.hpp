#pragma once
#include "TBoolConstant.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    struct TIsFloating : public TBoolConstant<(T)3/(T)2 != (T)1> {};
}
