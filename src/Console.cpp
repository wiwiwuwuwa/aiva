#include "Console.hpp"

#include "ManageObject.hpp"
#include "Process.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Console;


namespace
{
    class System final
    {
    public:
        System();

        void Print(CstrView const message) const;
        void Error(CstrView const message) const;

    private:
        void* m_printHandle{};
        void* m_errorHandle{};
    };
}
// namespace


static SpinLock GSystemLock{};
static ManageObject<System> GSystem{};


System::System()
{
    m_printHandle = WinApi::GetStdHandle(WinApi::STD_OUTPUT_HANDLE);
    if (!m_printHandle)
        Process::ExitFailure();

    m_errorHandle = WinApi::GetStdHandle(WinApi::STD_ERROR_HANDLE);
    if (!m_errorHandle)
        Process::ExitFailure();
}


void System::Print(CstrView const message) const
{
    auto written = uint32_t{};
    if (!WinApi::WriteFile(m_printHandle, message, StrLen(message), &written, nullptr))
        Process::ExitFailure();
}


void System::Error(CstrView const message) const
{
    auto written = uint32_t{};
    if (!WinApi::WriteFile(m_errorHandle, message, StrLen(message), &written, nullptr))
        Process::ExitFailure();
}


void Console::InitSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem.Construct();
}


void Console::ShutSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem.Destruct();
}


void Console::Print(CstrView const message)
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem->Print(message);
}


void Console::PrintLine(CstrView const message)
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem->Print(message);
    GSystem->Print("\n");
}


void Console::Error(CstrView const message)
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem->Error("\033[31m");
    GSystem->Error(message);
    GSystem->Error("\033[0m");
}


void Console::ErrorLine(CstrView const message)
{
    SpinLockScope_t const lockScope{ GSystemLock };
    GSystem->Error("\033[31m");
    GSystem->Error(message);
    GSystem->Error("\033[0m");
    GSystem->Error("\n");
}
