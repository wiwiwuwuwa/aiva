#pragma once
#include "ArithmeticTypes.hpp"

namespace Aiva::MinLib
{
    template <uint32_t Size>
    class StaticString final
    {
    public:
        constexpr StaticString() noexcept;
        constexpr StaticString(char const*const string) noexcept;
        constexpr StaticString& operator=(char const*const string) noexcept;
        constexpr operator char const*() const noexcept;
        constexpr operator char*() noexcept;
        constexpr uint32_t GetLength() const noexcept;

    private:
        char m_data[Size]{};
    };
}
// namespace Aiva::MinLib

#include "../../Sources/StaticString.inl"
