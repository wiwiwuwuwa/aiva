#pragma once
#include <MinLib/StaticString.hpp>
#include <MinLib/Algorithm.hpp>

namespace Aiva::MinLib
{
    template <uintptr_t Size>
    constexpr StaticString<Size>::StaticString() noexcept
    {
        m_data[0] = '\0';
    }

    template <uintptr_t Size>
    constexpr StaticString<Size>::StaticString(char const*const string) noexcept
    {
        if (!string)
            { m_data[0] = '\0'; return; }

        uintptr_t i{};

        for (; i < Size - 1 && string[i] != '\0'; i++)
            m_data[i] = string[i];

        m_data[i] = '\0';
    }

    template <uintptr_t Size>
    constexpr StaticString<Size>& StaticString<Size>::operator=(char const*const string) noexcept
    {
        return *this = StaticString(string);
    }

    template <uintptr_t Size>
    constexpr StaticString<Size>::operator char const*() const noexcept
    {
        return m_data;
    }

    template <uintptr_t Size>
    constexpr StaticString<Size>::operator char*() noexcept
    {
        return m_data;
    }

    template <uintptr_t Size>
    constexpr uintptr_t StaticString<Size>::GetLength() const noexcept
    {
        uintptr_t i{};

        for (; i < Size - 1 && m_data[i] != '\0'; i++);

        return i;
    }
}
