#include <SysLib/Console.hpp>
#include <MinLib/Algorithm.hpp>
#include <MinLib/ArithmeticTypes.hpp>
#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;
using namespace SysLib;

void Console::WriteToOutput_Impl(char const*const value)
{
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);

    auto written = uint32_t{};
    WriteFile(handle, value, GetStringLength(value), &written, nullptr);
}

void Console::WriteToError_Impl(char const*const value)
{
    const auto handle = GetStdHandle(STD_ERROR_HANDLE);

    auto written = uint32_t{};
    WriteFile(handle, value, GetStringLength(value), &written, nullptr);
}
