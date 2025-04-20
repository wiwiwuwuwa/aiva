#pragma once

#include "Numbers.hpp"
#include "NonCopyable.hpp"


namespace Aiva::Windows
{
    class IWindow
    {
    protected:
        virtual ~IWindow() = default;
    };


    void InitSystem();
    void ShutSystem();

    IWindow const& GetMainWindow();
}
// namespace Aiva::Windows
