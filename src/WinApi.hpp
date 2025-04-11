#pragma once
#include "Numbers.hpp"

extern "C" { namespace Aiva::WinApi
{
    inline auto const INVALID_HANDLE_VALUE = (void*)(uintptr_t)(-1);
    constexpr auto const STD_INPUT_HANDLE = (uint32_t)(-10);
    constexpr auto const STD_OUTPUT_HANDLE = (uint32_t)(-11);
    constexpr auto const STD_ERROR_HANDLE = (uint32_t)(-12);
    constexpr auto const INFINITE = (uint32_t)(-1);


    struct SYSTEM_INFO
    {
        union
        {
            uint32_t dwOemId;
            struct
            {
                uint16_t wProcessorArchitecture;
                uint16_t wReserved;
            } NONAME;
        } NONAME;
        uint32_t dwPageSize;
        void* lpMinimumApplicationAddress;
        void* lpMaximumApplicationAddress;
        uintptr_t dwActiveProcessorMask;
        uint32_t dwNumberOfProcessors;
        uint32_t dwProcessorType;
        uint32_t dwAllocationGranularity;
        uint16_t wProcessorLevel;
        uint16_t wProcessorRevision;
    };


    void* __stdcall CreateThread(void *const lpThreadAttributes, size_t const dwStackSize, uint32_t (__stdcall *lpStartAddress)(void*), void *const lpParameter, uint32_t const dwCreationFlags, uint32_t *const lpThreadId);
    [[noreturn]] void __stdcall ExitProcess(uint32_t const uExitCode);
    uint32_t __stdcall GetLastError();
    void* __stdcall GetStdHandle(uint32_t const nStdHandle);
    void __stdcall GetSystemInfo(SYSTEM_INFO* const lpSystemInfo);
    void* __stdcall HeapAlloc(void *const hHeap, uint32_t const dwFlags, size_t const dwBytes);
    void* __stdcall HeapCreate(uint32_t const flOptions, size_t const dwInitialSize, size_t const dwMaximumSize);
    int __stdcall HeapDestroy(void *const hHeap);
    int __stdcall HeapFree(void *const hHeap, uint32_t const dwFlags, void *const lpMem);
    void* __stdcall HeapReAlloc(void* const hHeap, uint32_t const dwFlags, void* const lpMem, size_t const dwBytes);
    int __stdcall TerminateThread(void *const hThread, uint32_t const uExitCode);
    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, uint32_t const nNumberOfBytesToWrite, uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);
}}
// extern "C" { namespace Aiva
