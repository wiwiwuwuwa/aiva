#pragma once
#include "Numbers.hpp"


extern "C" { namespace Aiva::WinApi
{
    //----------------------------------------------------------------
    // Base Types
    //----------------------------------------------------------------

    using BYTE = uint8_t;
    using WORD = uint16_t;
    using DWORD = uint32_t;
    using UINT = uint32_t;
    using LONG = int32_t;
    using BOOL = int32_t;
    using ATOM = WORD;
    using HANDLE = void*;
    using LPVOID = void*;
    using LPCVOID = void const*;
    using LPDWORD = DWORD*;
    using LPBOOL = BOOL*;
    using SIZE_T = size_t;
    using ULONG_PTR = uintptr_t;
    using UINT_PTR = uintptr_t;
    using LONG_PTR = intptr_t;

    using WCHAR = wchar_t;
    using LPCWSTR = WCHAR const*;
    using LPWSTR = WCHAR*;
    using HMODULE = HANDLE;
    using HINSTANCE = HANDLE;
    using HWND = HANDLE;
    using HMENU = HANDLE;
    using HMONITOR = HANDLE;

    using LRESULT = LONG_PTR;
    using WPARAM = UINT_PTR;
    using LPARAM = LONG_PTR;

    using LPTHREAD_START_ROUTINE = DWORD(__attribute__((stdcall))*)(LPVOID);
    using LPFIBER_START_ROUTINE = void (__attribute__((stdcall))*)(LPVOID);
    using WNDPROC = LRESULT(__attribute__((stdcall))*)(HWND, UINT, WPARAM, LPARAM);

    //----------------------------------------------------------------
    // Structures
    //----------------------------------------------------------------

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

    struct POINT
    {
        LONG x;
        LONG y;
    };
    using LPPOINT = POINT*;

    struct MSG
    {
        HWND hwnd;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
        DWORD time;
        POINT pt;
    };
    using LPMSG = MSG*;

    struct RECT
    {
        LONG left;
        LONG top;
        LONG right;
        LONG bottom;
    };
    using LPRECT = RECT*;

    struct WNDCLASSEXW
    {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        HANDLE hIcon;
        HANDLE hCursor;
        HANDLE hbrBackground;
        LPCWSTR lpszMenuName;
        LPCWSTR lpszClassName;
        HANDLE hIconSm;
    };

    struct MONITORINFO
    {
        DWORD cbSize;
        RECT rcMonitor;
        RECT rcWork;
        DWORD dwFlags;
    };
    using LPMONITORINFO = MONITORINFO*;

    //----------------------------------------------------------------
    // Constants
    //----------------------------------------------------------------

    static const HANDLE INVALID_HANDLE_VALUE = (HANDLE)(ULONG_PTR)(-1);
    constexpr DWORD STD_INPUT_HANDLE = (DWORD)(-10);
    constexpr DWORD STD_OUTPUT_HANDLE = (DWORD)(-11);
    constexpr DWORD STD_ERROR_HANDLE = (DWORD)(-12);
    constexpr DWORD INFINITE = (DWORD)(-1);
    constexpr DWORD TLS_OUT_OF_INDEXES = (DWORD)(-1);
    constexpr DWORD WAIT_FAILED = (DWORD)(-1);

    constexpr DWORD WS_OVERLAPPED = 0x00000000L;
    constexpr DWORD WS_CAPTION = 0x00C00000L;
    constexpr DWORD WS_SYSMENU = 0x00080000L;
    constexpr DWORD WS_THICKFRAME = 0x00040000L;
    constexpr DWORD WS_MINIMIZEBOX = 0x00020000L;
    constexpr DWORD WS_MAXIMIZEBOX = 0x00010000L;
    constexpr DWORD WS_OVERLAPPEDWINDOW = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
    constexpr DWORD WS_VISIBLE = 0x10000000L;

    constexpr DWORD MONITOR_DEFAULTTONULL = 0x00000000;
    constexpr DWORD MONITOR_DEFAULTTOPRIMARY = 0x00000001;
    constexpr DWORD MONITOR_DEFAULTTONEAREST = 0x00000002;

    static const HWND HWND_TOP = (HWND)0;
    static const HWND HWND_BOTTOM = (HWND)1;
    static const HWND HWND_TOPMOST = (HWND)-1;
    static const HWND HWND_NOTOPMOST = (HWND)-2;

    constexpr UINT SWP_NOSIZE = 0x0001;
    constexpr UINT SWP_NOMOVE = 0x0002;
    constexpr UINT SWP_NOZORDER = 0x0004;
    constexpr UINT SWP_NOREDRAW = 0x0008;
    constexpr UINT SWP_NOACTIVATE = 0x0010;
    constexpr UINT SWP_FRAMECHANGED = 0x0020;
    constexpr UINT SWP_SHOWWINDOW = 0x0040;
    constexpr UINT SWP_HIDEWINDOW = 0x0080;
    constexpr UINT SWP_NOCOPYBITS = 0x0100;
    constexpr UINT SWP_NOOWNERZORDER = 0x0200;
    constexpr UINT SWP_NOSENDCHANGING = 0x0400;

    constexpr UINT PM_NOREMOVE = 0x0000;
    constexpr UINT PM_REMOVE = 0x0001;
    constexpr UINT PM_NOYIELD = 0x0002;

    //----------------------------------------------------------------
    // Thread & Process Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
    __attribute__((dllimport, stdcall)) BOOL CloseHandle(HANDLE hObject);
    __attribute__((dllimport, stdcall)) DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT uExitCode);

    //----------------------------------------------------------------
    // Thread Local Storage (TLS)
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) DWORD TlsAlloc(void);
    __attribute__((dllimport, stdcall)) BOOL TlsFree(DWORD dwTlsIndex);
    __attribute__((dllimport, stdcall)) LPVOID TlsGetValue(DWORD dwTlsIndex);
    __attribute__((dllimport, stdcall)) BOOL TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue);

    //----------------------------------------------------------------
    // Standard Handles & File I/O
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HANDLE GetStdHandle(DWORD nStdHandle);
    __attribute__((dllimport, stdcall)) BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

    //----------------------------------------------------------------
    // Heap Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HANDLE HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
    __attribute__((dllimport, stdcall)) BOOL HeapDestroy(HANDLE hHeap);
    __attribute__((dllimport, stdcall)) LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
    __attribute__((dllimport, stdcall)) BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

    //----------------------------------------------------------------
    // System Information
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);

    //----------------------------------------------------------------
    // Fiber Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) LPVOID ConvertThreadToFiber(LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) BOOL ConvertFiberToThread(void);
    __attribute__((dllimport, stdcall)) LPVOID CreateFiber(SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) void SwitchToFiber(LPVOID lpFiber);
    __attribute__((dllimport, stdcall)) void DeleteFiber(LPVOID lpFiber);

    //----------------------------------------------------------------
    // Module/Instance Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HMODULE GetModuleHandleW(LPCWSTR lpModuleName);

    //----------------------------------------------------------------
    // Window Class Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) ATOM RegisterClassExW(const WNDCLASSEXW *lpwcx);
    __attribute__((dllimport, stdcall)) BOOL UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance);
    __attribute__((dllimport, stdcall)) LRESULT DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    //----------------------------------------------------------------
    // Window Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HWND CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
    __attribute__((dllimport, stdcall)) BOOL DestroyWindow(HWND hWnd);
    __attribute__((dllimport, stdcall)) BOOL IsWindow(HWND hWnd);
    __attribute__((dllimport, stdcall)) BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);
    __attribute__((dllimport, stdcall)) BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

    //----------------------------------------------------------------
    // Message Handling
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
    __attribute__((dllimport, stdcall)) BOOL TranslateMessage(const MSG *lpMsg);
    __attribute__((dllimport, stdcall)) LRESULT DispatchMessageW(const MSG *lpMsg);

    //----------------------------------------------------------------
    // Display/Monitor Management
    //----------------------------------------------------------------

    __attribute__((dllimport, stdcall)) HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags);
    __attribute__((dllimport, stdcall)) BOOL GetMonitorInfoW(HMONITOR hMonitor, LPMONITORINFO lpmi);

}} // extern "C" { namespace Aiva::WinApi
