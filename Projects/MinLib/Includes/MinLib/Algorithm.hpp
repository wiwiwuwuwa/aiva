#pragma once

namespace Aiva::MinLib
{
    template <typename Type, typename... Types>
    constexpr Type Min(Type const& value, Types const&... values);

    template <typename Type, typename... Types>
    constexpr Type Max(Type const& value, Types const&... values);
}
// namespace Aiva::MinLib

#include "../../Sources/Algorithm.inl"
