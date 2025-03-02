#pragma once
#include <MinLib/BoolConstant.hpp>

namespace Aiva::MinLib
{
    template <typename T>
    struct IsFloating : public BoolConstant<(T)3/(T)2 != (T)1> {};
}
// namespace Aiva::MinLib
