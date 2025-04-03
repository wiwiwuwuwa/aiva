#pragma once
#include "numbers.hpp"
#include "strings.hpp"

namespace aiva
{
    [[noreturn]] void ExitProcess(uintptr_t const code, CstrView const message);
    [[noreturn]] void ExitProcess();
    [[noreturn]] void ExitProcess(uintptr_t const code);
    [[noreturn]] void ExitProcess(CstrView const message);
}
// namespace aiva
