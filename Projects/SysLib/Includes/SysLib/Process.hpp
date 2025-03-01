#pragma once
#include <SysLib/ErrorCode.hpp>

namespace Aiva::SysLib
{
    class Process final
    {
    public:
        static void Exit(ErrorCode const errorCode) noexcept;

    private:
        Process() = delete;
    };
}
// namespace Aiva::SysLib
