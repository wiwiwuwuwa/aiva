#pragma once
#include <SysLib/ArithmeticTypes.hpp>

namespace Aiva::SysLib
{
    class Window final
    {
    public:
        Window(uint32_t const width, uint32_t const height);
        ~Window();

        operator bool() const;

    private:
        uint32_t m_handle{};
    };
}
