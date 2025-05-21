#include "Console_Minimal.hpp"

#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "Process.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;


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


    SpinLock GPrinterLock;
    ManageObject<Printer> GPrinter;
}
// namespace


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


void Console::Minimal::InitSystem()
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (GPrinter)
        Process::ExitFailure();

    GPrinter.Construct();
}


void Console::Minimal::ShutSystem()
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    GPrinter.Destruct();
}


void Console::Minimal::Print(Span<const CstrView> const& messages)
{
    SpinLockScope_t const lockScope{ GPrinterLock };

    if (!GPrinter)
        Process::ExitFailure();

    for (auto i = size_t{}; i < messages.GetSize(); i++)
        GPrinter->Print(messages[i]);
}


void Console::Minimal::Error(Span<const CstrView> const& messages)
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
