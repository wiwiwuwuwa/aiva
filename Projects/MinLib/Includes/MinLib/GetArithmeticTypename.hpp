#pragma once
#include <MinLib/GetDefaultArithmeticTypenameValue.hpp>
#include <MinLib/TypeIdentity.hpp>

namespace Aiva::MinLib
{
    template <ArithmeticSign Sign, ArithmeticSize Size, ArithmeticType Type>
    struct GetArithmeticTypename : public TypeIdentity<decltype(GetDefaultArithmeticTypenameValue<Sign, Size, Type>())> {};
}
// namespace Aiva::MinLib
