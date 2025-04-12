#pragma once
#include "Numbers.hpp"

extern "C" { namespace Aiva::WinApi
{
    inline auto const INVALID_HANDLE_VALUE = (void*)(uintptr_t)(-1);
    constexpr auto STD_INPUT_HANDLE = (uint32_t)(-10);
    constexpr auto STD_OUTPUT_HANDLE = (uint32_t)(-11);
    constexpr auto STD_ERROR_HANDLE = (uint32_t)(-12);
    constexpr auto INFINITE = (uint32_t)(-1);
    constexpr auto WAIT_FAILED = (uint32_t)(-1);


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


    void* __stdcall ConvertThreadToFiber(void *const lpParameter);
    int __stdcall ConvertFiberToThread();
    void* __stdcall CreateFiber(size_t const dwStackSize, void *const lpStartAddress, void *const lpParameter);
    void* __stdcall CreateThread(void *const lpThreadAttributes, size_t const dwStackSize, uint32_t (__stdcall *lpStartAddress)(void*), void *const lpParameter, uint32_t const dwCreationFlags, uint32_t *const lpThreadId);
    [[noreturn]] void __stdcall ExitProcess(uint32_t const uExitCode);
    void* __stdcall GetStdHandle(uint32_t const nStdHandle);
    void __stdcall GetSystemInfo(SYSTEM_INFO* const lpSystemInfo);
    void* __stdcall HeapAlloc(void *const hHeap, uint32_t const dwFlags, size_t const dwBytes);
    void* __stdcall HeapCreate(uint32_t const flOptions, size_t const dwInitialSize, size_t const dwMaximumSize);
    int __stdcall HeapDestroy(void *const hHeap);
    int __stdcall HeapFree(void *const hHeap, uint32_t const dwFlags, void *const lpMem);
    void __stdcall SwitchToFiber(void *const lpFiber);
    uint32_t __stdcall WaitForSingleObject(void *const hHandle, uint32_t const dwMilliseconds);
    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, uint32_t const nNumberOfBytesToWrite, uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);
}}
// extern "C" { namespace Aiva
