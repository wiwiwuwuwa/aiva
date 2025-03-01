#pragma once
#include <SysLib/ArithmeticTypes.hpp>

namespace Aiva::SysLib
{
    enum class ErrorCode : uint32_t
    {
        Unknown = static_cast<uint32_t>(-1),
        Success = 0,
    };
}
// namespace Aiva::SysLib
