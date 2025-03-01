#include <SysLib/Process.hpp>
#include <WinApi.hpp>
#include "ErrorCode.hpp"

using namespace Aiva;
using namespace WinApi;
using namespace SysLib;

void Process::Exit(ErrorCode const errorCode) noexcept
{
    const auto winApiCode = ErrorCodeToWinApiCode(errorCode);
    ExitProcess(winApiCode);
}
