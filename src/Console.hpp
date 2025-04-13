#pragma once
#include "CstrView.hpp"
#include "Span.hpp"


namespace Aiva::Console
{
    void InitSystem();
    void ShutSystem();

    void Print(CstrView const message);
    void PrintLine(CstrView const message);
    void Print(Span<const CstrView> const messages);
    void PrintLine(Span<const CstrView> const messages);
    void Error(CstrView const message);
    void ErrorLine(CstrView const message);
    void Error(Span<const CstrView> const messages);
    void ErrorLine(Span<const CstrView> const messages);
}
// namespace Aiva
