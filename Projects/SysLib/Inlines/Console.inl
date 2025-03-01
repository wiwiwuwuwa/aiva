#include <SysLib/Console.hpp>

namespace Aiva::SysLib
{
    template <typename... Types>
    void Console::WriteToOutput(Types const&... values)
    {
        (WriteToOutput_Impl(values), ...);
    }

    template <typename... Types>
    void Console::WriteToError(Types const&... values)
    {
        (WriteToError_Impl(values), ...);
    }
}
// Aiva::SysLib::Console
