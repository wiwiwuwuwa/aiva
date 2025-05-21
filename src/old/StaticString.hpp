#pragma once
#include "CstrView.hpp"
#include "Numbers.hpp"
#include "Templates.hpp"


namespace Aiva
{
    template <size_t TSize>
    class StaticString final
    {
        static_assert(TSize > 0);

    public:
        constexpr StaticString();
        constexpr operator CstrView() const;
        constexpr size_t StrLen() const;
        constexpr char& operator[](size_t const index);
        constexpr StaticString& operator+=(char const chr);

    private:
        size_t m_length;
        char m_buffer[TSize];
    };


    using StaticString256_t = StaticString<256>;


    template <typename TType, Templates::EnableIf_t<Templates::IsInteger_v<TType>> = true>
    constexpr StaticString256_t ToStaticString(TType const value);
}
// namespace Aiva


#include "StaticString.inl"
