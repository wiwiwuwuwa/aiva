#pragma once
#include <MinLib.hpp>

extern "C" { namespace Aiva::WinApi
{
    // HANDLE WINAPI GetStdHandle
    // (
        // _In_ DWORD nStdHandle
    // );
    void* __stdcall GetStdHandle(MinLib::uint32_t const nStdHandle);

    // BOOL WriteFile
    // (
        // [in]                HANDLE       hFile,
        // [in]                LPCVOID      lpBuffer,
        // [in]                DWORD        nNumberOfBytesToWrite,
        // [out, optional]     LPDWORD      lpNumberOfBytesWritten,
        // [in, out, optional] LPOVERLAPPED lpOverlapped
    // );
    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, MinLib::uint32_t const nNumberOfBytesToWrite, MinLib::uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);

    // void ExitProcess
    // (
        // [in] UINT uExitCode
    // );
    void __stdcall ExitProcess(MinLib::uint32_t const uExitCode);
}}
// extern "C" namespace Aiva::WinApi
