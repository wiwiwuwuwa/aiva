#pragma once

#include "CstrView.hpp"
#include "Span.hpp"


namespace Aiva::Console::Minimal
{
    void InitSystem();
    void ShutSystem();

    void Print(Span<const CstrView> const& messages);
    void Error(Span<const CstrView> const& messages);
}
// namespace Aiva
