#include "Console.hpp"

#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "Process.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Console;


namespace
{
    class Printer final : public NonCopyable
    {
    public:
        Printer();

        void Print(CstrView const message) const;
        void Error(CstrView const message) const;

    private:
        void* m_printHandle;
        void* m_errorHandle;
    };
}
// namespace


static SpinLock GPrinterLock;
static ManageObject<Printer> GPrinter;


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

    if (GPrinter)
        Process::ExitFailure();

    GPrinter.Construct();
}


void Console::ShutSystem()
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    GPrinter.Destruct();
}


void Console::Print(CstrView const message)
{
    Print(Span<const CstrView>{ message });
}


void Console::PrintLine(CstrView const message)
{
    PrintLine(Span<const CstrView>{ message });
}


void Console::Error(CstrView const message)
{
    Error(Span<const CstrView>{ message });
}


void Console::ErrorLine(CstrView const message)
{
    ErrorLine(Span<const CstrView>{ message });
}


void Console::Print(Span<const CstrView> const& messages)
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    for (auto i = size_t{}; i < messages.GetSize(); i++)
        GPrinter->Print(messages[i]);
}


void Console::PrintLine(Span<const CstrView> const& messages)
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    for (auto i = size_t{}; i < messages.GetSize(); i++)
        GPrinter->Print(messages[i]);

    GPrinter->Print("\n");
}


void Console::Error(Span<const CstrView> const& messages)
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    for (auto i = size_t{}; i < messages.GetSize(); i++)
    {
        GPrinter->Error("\033[31m");
        GPrinter->Error(messages[i]);
        GPrinter->Error("\033[0m");
    }
}


void Console::ErrorLine(Span<const CstrView> const& messages)
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    for (auto i = size_t{}; i < messages.GetSize(); i++)
    {
        GPrinter->Error("\033[31m");
        GPrinter->Error(messages[i]);
        GPrinter->Error("\033[0m");
    }

    GPrinter->Error("\n");
}
