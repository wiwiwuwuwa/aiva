#pragma once
#include "strings.hpp"


namespace aiva::Console
{
    void Print(CstrView const message);
    void PrintLine(CstrView const message);
    void Error(CstrView const message);
    void ErrorLine(CstrView const message);
}
// namespace aiva
