#include <SysLib/Process.hpp>
#include <WinApi.hpp>
#include "ErrorCodeUtils.hpp"

using namespace Aiva;
using namespace WinApi;
using namespace SysLib;

void Process::Exit(ErrorCode const errorCode) noexcept
{
    auto const winApiCode = ErrorCodeToWinApiCode(errorCode);
    ExitProcess(winApiCode);
}
