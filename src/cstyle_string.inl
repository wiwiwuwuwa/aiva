#pragma once
#include "cstyle_string.hpp"

namespace aiva
{
    constexpr size_t StrLen(char const*const str)
    {
        auto length = size_t{};

        if (!str)
            return length;

        while (str[length] != '\0')
            length++;

        return length;
    }
}
// namespace aiva
