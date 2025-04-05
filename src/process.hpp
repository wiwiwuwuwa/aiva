#pragma once
#include "numbers.hpp"
#include "strings.hpp"


namespace aiva::Process
{
    [[noreturn]] void Exit(uintptr_t const code = {}, CstrView const message = {});
    [[noreturn]] void ExitSuccess(CstrView const message = {});
    [[noreturn]] void ExitFailure(CstrView const message = {});
}
// namespace aiva
