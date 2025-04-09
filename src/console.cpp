#include "console.hpp"
#include "ensures.hpp"
#include "memory.hpp"
#include "strings.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Console;


namespace
{
    struct System final
    {
        void* printHandle{};
        void* errorHandle{};
    };
}
// namespace


static Memory::MemoryAsObject<System> GSystemObject{};
static System* GSystem{};


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
    if (GSystem)
        CheckNoEntry();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();

    GSystem->printHandle = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
    if (!GSystem->printHandle)
        CheckNoEntry();

    GSystem->errorHandle = winapi::GetStdHandle(winapi::STD_ERROR_HANDLE);
    if (!GSystem->errorHandle)
        CheckNoEntry();
}


void Console::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    GSystem = nullptr;
    GSystemObject.Destruct();
}


void Console::Print(CstrView const message)
{
    if (!GSystem)
        CheckNoEntry();

    if (message)
        Write(GSystem->printHandle, message);
}


void Console::PrintLine(CstrView const message)
{
    if (!GSystem)
        CheckNoEntry();

    if (message)
        Print(message);

    Print("\n");
}


void Console::Error(CstrView const message)
{
    if (!GSystem)
        CheckNoEntry();

    if (message)
    {
        Write(GSystem->errorHandle, "\033[31m");
        Write(GSystem->errorHandle, message);
        Write(GSystem->errorHandle, "\033[0m");
    }
}


void Console::ErrorLine(CstrView const message)
{
    if (!GSystem)
        CheckNoEntry();

    if (message)
        Error(message);

    Error("\n");
}
