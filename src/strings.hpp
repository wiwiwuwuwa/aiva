#pragma once
#include "numbers.hpp"

namespace aiva
{
    class CstrView final
    {
    public:
        constexpr CstrView();
        constexpr CstrView(char const*const data);
        constexpr operator bool() const;
        constexpr operator char const*() const;
        constexpr char const* c_str() const;
        constexpr char operator[](size_t const index) const;

    private:
        char const*const m_data;
    };

    constexpr size_t StrLen(CstrView const str);
}
// namespace aiva

#include "strings.inl"
