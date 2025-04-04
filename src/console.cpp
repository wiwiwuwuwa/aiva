#include "console.hpp"
#include "strings.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Console;


static void Write(void*const handle, CstrView const message)
{
    if (handle == winapi::INVALID_HANDLE_VALUE)
        winapi::ExitProcess(1);

    if (!message)
        return;

    auto written = uint32_t{};

    if (!winapi::WriteFile(handle, message, StrLen(message), &written, nullptr))
        winapi::ExitProcess(winapi::GetLastError());
}


void Console::Print(CstrView const message)
{
    if (!message)
        return;

    auto const handle = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
    if (handle == winapi::INVALID_HANDLE_VALUE)
        winapi::ExitProcess(winapi::GetLastError());

    Write(handle, message);
}


void Console::PrintLine(CstrView const message)
{
    auto const handle = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
    if (handle == winapi::INVALID_HANDLE_VALUE)
        winapi::ExitProcess(winapi::GetLastError());

    Write(handle, message);
    Write(handle, "\n");
}


void Console::Error(CstrView const message)
{
    if (!message)
        return;

    auto const handle = winapi::GetStdHandle(winapi::STD_ERROR_HANDLE);
    if (handle == winapi::INVALID_HANDLE_VALUE)
        winapi::ExitProcess(winapi::GetLastError());

    Write(handle, "\033[31m");
    Write(handle, message);
    Write(handle, "\033[0m");
}


void Console::ErrorLine(CstrView const message)
{
    auto const handle = winapi::GetStdHandle(winapi::STD_ERROR_HANDLE);
    if (handle == winapi::INVALID_HANDLE_VALUE)
        winapi::ExitProcess(winapi::GetLastError());

    Write(handle, "\033[31m");
    Write(handle, message);
    Write(handle, "\033[0m");
    Write(handle, "\n");
}
