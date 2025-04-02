#include "console.hpp"
#include "cstyle_string.hpp"
#include "process.hpp"
#include "winapi.hpp"

namespace aiva
{
    void Print(char const*const str)
    {
        auto const handle = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
        if (handle == winapi::INVALID_HANDLE_VALUE)
            ExitProcess(1);

        auto written = uint32_t{};
        if (!winapi::WriteFile(handle, str, StrLen(str), &written, nullptr))
            ExitProcess(1);
    }

    void PrintLine(char const*const str)
    {
        Print(str);
        Print("\n");
    }

    void Error(char const*const str)
    {
        auto const handle = winapi::GetStdHandle(winapi::STD_ERROR_HANDLE);
        if (handle == winapi::INVALID_HANDLE_VALUE)
            ExitProcess(1);

        auto written = uint32_t{};
        if (!winapi::WriteFile(handle, str, StrLen(str), &written, nullptr))
            ExitProcess(1);
    }

    void ErrorLine(char const*const str)
    {
        Error(str);
        Error("\n");
    }
}
// namespace aiva
