#pragma once
#include <WinApi.hpp>
#include <SysLib/ErrorCode.hpp>

namespace Aiva::SysLib
{
	constexpr uint32_t ErrorCodeToWinApiCode(ErrorCode const errorCode);

	constexpr ErrorCode WinApiCodeToErrorCode(uint32_t const winApiCode);
}
// Aiva::SysLib

#include "ErrorCodeUtils.inl"
