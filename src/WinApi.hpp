#pragma once
#include "Numbers.hpp"


extern "C" { namespace Aiva::WinApi
{
    using BYTE = uint8_t;
    using WORD = uint16_t;
    using DWORD = uint32_t;
    using UINT = uint32_t;
    using LONG = int32_t;
    using BOOL = int32_t;
    using HANDLE = void*;
    using LPVOID = void*;
    using LPCVOID = void const*;
    using LPDWORD = DWORD*;
    using LPBOOL = BOOL*;
    using SIZE_T = size_t;
    using ULONG_PTR = uintptr_t;

    using LPTHREAD_START_ROUTINE = DWORD(__attribute__((stdcall))*)(LPVOID);
    using LPFIBER_START_ROUTINE = void (__attribute__((stdcall))*)(LPVOID);

    struct SECURITY_ATTRIBUTES final
    {
        DWORD nLength;
        LPVOID lpSecurityDescriptor;
        BOOL bInheritHandle;
    };
    using LPSECURITY_ATTRIBUTES = SECURITY_ATTRIBUTES*;

    struct SYSTEM_INFO
    {
        union
        {
            DWORD dwOemId;
            struct
            {
                WORD wProcessorArchitecture;
                WORD wReserved;
            } DUMMY;
        } DUMMYUNION;
        DWORD dwPageSize;
        LPVOID lpMinimumApplicationAddress;
        LPVOID lpMaximumApplicationAddress;
        ULONG_PTR dwActiveProcessorMask;
        DWORD dwNumberOfProcessors;
        DWORD dwProcessorType;
        DWORD dwAllocationGranularity;
        WORD wProcessorLevel;
        WORD wProcessorRevision;
    };
    using LPSYSTEM_INFO = SYSTEM_INFO*;

    struct OVERLAPPED;
    using LPOVERLAPPED = OVERLAPPED*;

    static const HANDLE INVALID_HANDLE_VALUE = (HANDLE)(ULONG_PTR)(-1);
    constexpr DWORD STD_INPUT_HANDLE = (DWORD)(-10);
    constexpr DWORD STD_OUTPUT_HANDLE = (DWORD)(-11);
    constexpr DWORD STD_ERROR_HANDLE = (DWORD)(-12);
    constexpr DWORD INFINITE = (DWORD)(-1);
    constexpr DWORD TLS_OUT_OF_INDEXES = (DWORD)(-1);
    constexpr DWORD WAIT_FAILED = (DWORD)(-1);

    __attribute__((dllimport, stdcall)) HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
    __attribute__((dllimport, stdcall)) BOOL CloseHandle(HANDLE hObject);
    __attribute__((dllimport, stdcall)) DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

    __attribute__((dllimport, stdcall)) DWORD TlsAlloc(void);
    __attribute__((dllimport, stdcall)) BOOL TlsFree(DWORD dwTlsIndex);
    __attribute__((dllimport, stdcall)) LPVOID TlsGetValue(DWORD dwTlsIndex);
    __attribute__((dllimport, stdcall)) BOOL TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);

    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT uExitCode);

    __attribute__((dllimport, stdcall)) HANDLE GetStdHandle(DWORD nStdHandle);
    __attribute__((dllimport, stdcall)) BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

    __attribute__((dllimport, stdcall)) HANDLE HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
    __attribute__((dllimport, stdcall)) BOOL HeapDestroy(HANDLE hHeap);
    __attribute__((dllimport, stdcall)) LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
    __attribute__((dllimport, stdcall)) BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

    __attribute__((dllimport, stdcall)) void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);

    __attribute__((dllimport, stdcall)) LPVOID ConvertThreadToFiber(LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) BOOL ConvertFiberToThread(void);
    __attribute__((dllimport, stdcall)) LPVOID CreateFiber(SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) void SwitchToFiber(LPVOID lpFiber);
    __attribute__((dllimport, stdcall)) void DeleteFiber(LPVOID lpFiber);

}} // extern "C" { namespace Aiva::WinApi
