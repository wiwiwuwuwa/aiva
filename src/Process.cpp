#include "Process.hpp"
#include "Console.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Process;


[[noreturn]] void Process::ExitSuccess()
{
    WinApi::ExitProcess(0);
}


[[noreturn]] void Process::ExitFailure()
{
    WinApi::ExitProcess(1);
}
