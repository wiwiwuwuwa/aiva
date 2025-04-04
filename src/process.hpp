#pragma once
#include "numbers.hpp"
#include "strings.hpp"


namespace aiva::Process
{
    [[noreturn]] void Exit(uintptr_t const code, CstrView const message);
    [[noreturn]] void Exit();
    [[noreturn]] void Exit(uintptr_t const code);
    [[noreturn]] void Exit(CstrView const message);
}
// namespace aiva
