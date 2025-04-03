#include "process.hpp"
#include "console.hpp"
#include "winapi.hpp"

namespace aiva
{
    [[noreturn]] void ExitProcess(uintptr_t const code, CstrView const message)
    {
        if (message)
        {
            if (code == 0)
                PrintLine(message);
            else
                ErrorLine(message);
        }

        winapi::ExitProcess((uint32_t)code);
    }

    [[noreturn]] void ExitProcess()
    {
        ExitProcess({}, {});
    }

    [[noreturn]] void ExitProcess(uintptr_t const code)
    {
        ExitProcess(code, {});
    }

    [[noreturn]] void ExitProcess(CstrView const message)
    {
        ExitProcess(1, message);
    }
}
// namespace aiva
