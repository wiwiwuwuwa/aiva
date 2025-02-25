#pragma once
#include "TGetDefaultArithmeticTypenameValue.hpp"
#include "TTypeIdentity.hpp"

namespace Aiva::MinLib
{
    template <EArithmeticSign Sign, EArithmeticSize Size, EArithmeticType Type>
    struct TGetArithmeticTypename : public TTypeIdentity<decltype(TGetDefaultArithmeticTypenameValue<Sign, Size, Type>())> {};
}
// namespace Aiva::MinLib
