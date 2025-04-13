#include "Console.hpp"

#include "ManageObject.hpp"
#include "Process.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Console;


namespace
{
    class Printer final
    {
    public:
        Printer();
        ~Printer() = default;

        Printer(Printer const&) = delete;
        Printer& operator=(Printer const&) = delete;
        Printer(Printer&&) = delete;
        Printer& operator=(Printer&&) = delete;

        void Print(CstrView const message) const;
        void Error(CstrView const message) const;

    private:
        void* m_printHandle;
        void* m_errorHandle;
    };
}
// namespace


static SpinLock GPrinterLock{};
static ManageObject<Printer> GPrinter{};


Printer::Printer()
{
    m_printHandle = WinApi::GetStdHandle(WinApi::STD_OUTPUT_HANDLE);
    if (!m_printHandle)
        Process::ExitFailure();

    m_errorHandle = WinApi::GetStdHandle(WinApi::STD_ERROR_HANDLE);
    if (!m_errorHandle)
        Process::ExitFailure();
}


void Printer::Print(CstrView const message) const
{
    auto written = uint32_t{};

    if (!WinApi::WriteFile(m_printHandle, message, StrLen(message), &written, nullptr))
        Process::ExitFailure();
}


void Printer::Error(CstrView const message) const
{
    auto written = uint32_t{};

    if (!WinApi::WriteFile(m_errorHandle, message, StrLen(message), &written, nullptr))
        Process::ExitFailure();
}


void Console::InitSystem()
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter.Construct();
}


void Console::ShutSystem()
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter.Destruct();
}


void Console::Print(CstrView const message)
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter->Print(message);
}


void Console::PrintLine(CstrView const message)
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter->Print(message);
    GPrinter->Print("\n");
}


void Console::Error(CstrView const message)
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter->Error("\033[31m");
    GPrinter->Error(message);
    GPrinter->Error("\033[0m");
}


void Console::ErrorLine(CstrView const message)
{
    SpinLockScope_t const lockScope{ GPrinterLock };
    GPrinter->Error("\033[31m");
    GPrinter->Error(message);
    GPrinter->Error("\033[0m");
    GPrinter->Error("\n");
}
