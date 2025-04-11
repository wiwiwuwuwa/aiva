#include "Console.hpp"
#include "Process.hpp"
#include "RawObject.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Console;


namespace
{
    struct System final
    {
        void* printHandle{};
        void* errorHandle{};
    };
}
// namespace


static RawObject<System> GSystemObject{};
static System* GSystem{};


static void Write(void*const handle, CstrView const message)
{
    auto written = uint32_t{};
    if (!WinApi::WriteFile(handle, message, StrLen(message), &written, nullptr))
        Process::ExitFailure();
}


void Console::InitSystem()
{
    if (GSystem)
        Process::ExitFailure();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();

    GSystem->printHandle = WinApi::GetStdHandle(WinApi::STD_OUTPUT_HANDLE);
    if (!GSystem->printHandle)
        Process::ExitFailure();

    GSystem->errorHandle = WinApi::GetStdHandle(WinApi::STD_ERROR_HANDLE);
    if (!GSystem->errorHandle)
        Process::ExitFailure();
}


void Console::ShutSystem()
{
    if (!GSystem)
        Process::ExitFailure();

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Console::Print(CstrView const message)
{
    if (!GSystem)
        Process::ExitFailure();

    Write(GSystem->printHandle, message);
}


void Console::PrintLine(CstrView const message)
{
    if (!GSystem)
        Process::ExitFailure();

    Print(message);
    Print("\n");
}


void Console::Error(CstrView const message)
{
    if (!GSystem)
        Process::ExitFailure();

    Write(GSystem->errorHandle, "\033[31m");
    Write(GSystem->errorHandle, message);
    Write(GSystem->errorHandle, "\033[0m");
}


void Console::ErrorLine(CstrView const message)
{
    if (!GSystem)
        Process::ExitFailure();

    Error(message);
    Error("\n");
}
