#pragma once
#include "../Includes/MinLib/StaticString.hpp"
#include "../Includes/MinLib/Algorithm.hpp"

namespace Aiva::MinLib
{
    template <uint32_t Size>
    constexpr StaticString<Size>::StaticString() noexcept
    {
        m_data[0] = '\0';
    }

    template <uint32_t Size>
    constexpr StaticString<Size>::StaticString(char const*const string) noexcept
    {
        if (!string)
            { m_data[0] = '\0'; return; }

        uint32_t i{};

        for (; i < Size - 1 && string[i] != '\0'; i++)
            m_data[i] = string[i];

        m_data[i] = '\0';
    }

    template <uint32_t Size>
    constexpr StaticString<Size>& StaticString<Size>::operator=(char const*const string) noexcept
    {
        return *this = StaticString(string);
    }

    template <uint32_t Size>
    constexpr StaticString<Size>::operator char const*() const noexcept
    {
        return m_data;
    }

    template <uint32_t Size>
    constexpr StaticString<Size>::operator char*() noexcept
    {
        return m_data;
    }

    template <uint32_t Size>
    constexpr uint32_t StaticString<Size>::GetLength() const noexcept
    {
        uint32_t i{};

        for (; i < Size - 1 && m_data[i] != '\0'; i++);

        return i;
    }
}
