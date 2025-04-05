#include "console.hpp"
#include "ensures.hpp"
#include "strings.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Console;


static bool GInitialized{};
static void* GPrintHandle;
static void* GErrorHandle;


static void Write(void*const handle, CstrView const message)
{
    if (handle == winapi::INVALID_HANDLE_VALUE)
        CheckNoEntry();

    if (!message)
        return;

    auto written = uint32_t{};

    if (!winapi::WriteFile(handle, message, StrLen(message), &written, nullptr))
        CheckNoEntry();
}


void Console::InitSystem()
{
    if (GInitialized)
        CheckNoEntry();

    GPrintHandle = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
    if (!GPrintHandle)
        CheckNoEntry();

    GErrorHandle = winapi::GetStdHandle(winapi::STD_ERROR_HANDLE);
    if (!GErrorHandle)
        CheckNoEntry();

    GInitialized = true;
}


void Console::ShutSystem()
{
    if (!GInitialized)
        CheckNoEntry();

    GInitialized = false;
}


void Console::Print(CstrView const message)
{
    if (!GInitialized)
        CheckNoEntry();

    if (message)
        Write(GPrintHandle, message);
}


void Console::PrintLine(CstrView const message)
{
    if (!GInitialized)
        CheckNoEntry();

    if (message)
        Print(message);

    Print("\n");
}


void Console::Error(CstrView const message)
{
    if (!GInitialized)
        CheckNoEntry();

    if (message)
    {
        Write(GErrorHandle, "\033[31m");
        Write(GErrorHandle, message);
        Write(GErrorHandle, "\033[0m");
    }
}


void Console::ErrorLine(CstrView const message)
{
    if (!GInitialized)
        CheckNoEntry();

    if (message)
        Error(message);

    Error("\n");
}
