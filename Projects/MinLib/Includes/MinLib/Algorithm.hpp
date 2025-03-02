#pragma once
#include "ArithmeticTypes.hpp"

namespace Aiva::MinLib
{
    template <typename Type, typename... Types>
    constexpr Type Min(Type const& value, Types const&... values);

    template <typename Type, typename... Types>
    constexpr Type Max(Type const& value, Types const&... values);

    constexpr uintptr_t GetStringLength(char const*const string);
}
// namespace Aiva::MinLib

#include "../../Inlines/Algorithm.inl"
