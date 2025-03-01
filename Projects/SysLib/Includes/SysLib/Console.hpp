#pragma once

namespace Aiva::SysLib
{
    class Console final
    {
    public:
        template <typename... Types>
        static void WriteToOutput(Types const&... values);

        template <typename... Types>
        static void WriteToError(Types const&... values);

    private:
        Console() = delete;

        static void WriteToOutput_Impl(char const*const value);

        static void WriteToError_Impl(char const*const value);
    };
}
// namespace Aiva::SysLib

#include "../../Inlines/Console.inl"
