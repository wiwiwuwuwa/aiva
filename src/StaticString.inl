#pragma once
#include "StaticString.hpp"

#include "Ensures.hpp"
#include "Math.hpp"


namespace Aiva
{
    template <size_t TSize>
    constexpr StaticString<TSize>::StaticString()
    {
        m_length = 0;
        m_buffer[0] = '\0';
    }


    template <size_t TSize>
    constexpr StaticString<TSize>::operator CstrView() const
    {
        return CstrView{ m_buffer };
    }


    template <size_t TSize>
    constexpr size_t StaticString<TSize>::StrLen() const
    {
        return m_length;
    }


    template <size_t TSize>
    constexpr char& StaticString<TSize>::operator[](size_t const index)
    {
        return m_buffer[index];
    }


    template <size_t TSize>
    constexpr StaticString<TSize>& StaticString<TSize>::operator+=(char const chr)
    {
        if (m_length >= TSize - 1)
            CheckNoEntry();

        m_buffer[m_length] = chr;
        m_length++;
        m_buffer[m_length] = '\0';

        return *this;
    }


    template <typename TType, Templates::EnableIf_t<Templates::IsInteger_v<TType>>>
    constexpr StaticString256_t ToStaticString(TType const value)
    {
        auto val = Math::Abs(value);
        auto rstr = StaticString256_t{};

        while (val > 0)
        {
            auto const dig = val % 10;
            val /= 10;

            auto const chr = '0' + dig;
            rstr += chr;
        }

        auto fstr = StaticString256_t{};

        if (value < 0)
            fstr += '-';

        for (auto i = rstr.StrLen(); i > 0; i--)
            fstr += rstr[i - 1];

        return fstr;
    }
}
