#pragma once
#include "Console.hpp"
#include "Console_Minimal.hpp"


namespace Aiva::Console
{
    template <typename... TArgs>
    void Print(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}... };
        Minimal::Print(messages);
    }

    template <typename... TArgs>
    void PrintLine(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Minimal::Print(messages);
    }

    template <typename... TArgs>
    void Error(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}... };
        Minimal::Error(messages);
    }

    template <typename... TArgs>
    void ErrorLine(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Minimal::Error(messages);
    }
}
// namespace Aiva::Console
