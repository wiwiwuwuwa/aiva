#pragma once
#include <SysLib/ArithmeticTypes.hpp>

namespace Aiva::SysLib
{
    class Window final
    {
    public:
        Window(uintptr_t const width, uintptr_t const height);
        ~Window();

        Window(Window const&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window const&) = delete;
        Window& operator=(Window&&) = delete;

        operator bool() const;

    private:
        uintptr_t m_handle{};
    };
}
