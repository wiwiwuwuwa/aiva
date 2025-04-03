#pragma once
#include "numbers.hpp"

extern "C" { namespace aiva::winapi
{
    inline auto const INVALID_HANDLE_VALUE = (void*)(uintptr_t)(-1);
    constexpr auto const STD_INPUT_HANDLE = (uint32_t)(-10);
    constexpr auto const STD_OUTPUT_HANDLE = (uint32_t)(-11);
    constexpr auto const STD_ERROR_HANDLE = (uint32_t)(-12);

    [[noreturn]] void __stdcall ExitProcess(uint32_t const uExitCode);
    uint32_t __stdcall GetLastError();
    void* __stdcall GetStdHandle(uint32_t const nStdHandle);
    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, uint32_t const nNumberOfBytesToWrite, uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);
}}
// extern "C" { namespace aiva
