#pragma once
#include "Numbers.hpp"
#include "CstrView.hpp"


namespace Aiva::Process
{
    [[noreturn]] void Exit(uintptr_t const code = {}, CstrView const message = {});
    [[noreturn]] void ExitSuccess(CstrView const message = {});
    [[noreturn]] void ExitFailure(CstrView const message = {});
}
// namespace Aiva
