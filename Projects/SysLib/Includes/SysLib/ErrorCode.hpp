#pragma once
#include <SysLib/ArithmeticTypes.hpp>

namespace Aiva::SysLib
{
    enum class ErrorCode : uintptr_t
    {
        Unknown = static_cast<uintptr_t>(-1),
        Success = 0,
    };
}
// namespace Aiva::SysLib
