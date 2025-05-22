#pragma once

#include "CstrView.hpp"
#include "Span.hpp"


namespace Aiva::Console
{
    void InitSystem();
    void ShutSystem();

    template <typename... TArgs>
    void Print(TArgs const&... args);

    template <typename... TArgs>
    void PrintLine(TArgs const&... args);

    template <typename... TArgs>
    void Error(TArgs const&... args);

    template <typename... TArgs>
    void ErrorLine(TArgs const&... args);
}
// namespace Aiva


#include "Console.inl"
