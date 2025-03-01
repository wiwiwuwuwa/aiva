#pragma once
#include <WinApi/ArithmeticTypes.hpp>

extern "C" { namespace Aiva::WinApi
{
    constexpr const uint32_t INVALID_HANDLE_VALUE = static_cast<uint32_t>(-1);
    constexpr const uint32_t ERROR_SUCCESS = 0;

    enum class StdHandleType : uint32_t
    {
        Output = static_cast<uint32_t>(-11),
        Error = static_cast<uint32_t>(-12),
    };

    void* __stdcall GetStdHandle(StdHandleType const stdHandleType);

    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, uint32_t const nNumberOfBytesToWrite, uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);

    void __stdcall ExitProcess(uint32_t const uExitCode);
}}
// extern "C" namespace Aiva::WinApi
