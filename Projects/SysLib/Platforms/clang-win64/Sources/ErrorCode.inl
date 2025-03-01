#include "ErrorCode.hpp"

namespace Aiva::SysLib
{
    constexpr uint32_t ErrorCodeToWinApiCode(ErrorCode const errorCode)
    {
        if (errorCode == ErrorCode::Unknown)
            return WinApi::INVALID_HANDLE_VALUE;

        if (errorCode == ErrorCode::Success)
            return WinApi::ERROR_SUCCESS;

        return WinApi::INVALID_HANDLE_VALUE;
    }

    constexpr ErrorCode WinApiCodeToErrorCode(uint32_t const winApiCode)
    {
        if (winApiCode == WinApi::INVALID_HANDLE_VALUE)
            return ErrorCode::Unknown;

        if (winApiCode == WinApi::ERROR_SUCCESS)
            return ErrorCode::Success;

        return ErrorCode::Unknown;
    }
}
// Aiva::SysLib
