#include "process.hpp"
#include "console.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Process;


[[noreturn]] void Process::Exit(uintptr_t const code, CstrView const message)
{
    if (message)
    {
        if (code == 0)
            Console::PrintLine(message);
        else
            Console::ErrorLine(message);
    }

    winapi::ExitProcess((uint32_t)code);
}


[[noreturn]] void Process::ExitSuccess(CstrView const message)
{
    Exit(0, message);
}


[[noreturn]] void Process::ExitFailure(CstrView const message)
{
    Exit(1, message);
}
