#pragma once
#include <WinApi/ArithmeticTypes.hpp>

extern "C" { namespace Aiva::WinApi
{
    constexpr const uint32_t INVALID_HANDLE_VALUE = static_cast<uint32_t>(-1);
    constexpr const uint32_t ERROR_SUCCESS = 0;

    constexpr const uint32_t STD_INPUT_HANDLE = static_cast<uint32_t>(-10);
    constexpr const uint32_t STD_OUTPUT_HANDLE = static_cast<uint32_t>(-11);
    constexpr const uint32_t STD_ERROR_HANDLE = static_cast<uint32_t>(-12);

    constexpr const uint32_t SW_HIDE = 0;
    constexpr const uint32_t SW_SHOWNORMAL = 1;

    constexpr const uint32_t WS_OVERLAPPEDWINDOW = 0x000000CF;

    using WNDPROC = int64_t(__stdcall*)(void* /*hWnd*/, uint32_t /*uMsg*/, uint64_t /*wParam*/, int64_t /*lParam*/);

    struct POINT final
    {
        int32_t x{};
        int32_t y{};
    };

    struct MSG final
    {
        void* hwnd{};
        uint32_t message{};
        uint64_t wParam{};
        int64_t lParam{};
        uint32_t time{};
        POINT pt{};
        uint32_t lPrivate{};
    };

    struct WNDCLASSEXW final
	{
		uint32_t cbSize{};
		uint32_t style{};
		WNDPROC lpfnWndProc{};
		int32_t cbClsExtra{};
		int32_t cbWndExtra{};
		void* hInstance{};
		void* hIcon{};
		void* hCursor{};
		void* hbrBackground{};
		wchar_t const* lpszMenuName{};
		wchar_t const* lpszClassName{};
		void* hIconSm{};
	};

    void* __stdcall CreateWindowExW(uint32_t const exStyle, wchar_t const*const className, wchar_t const*const windowName, uint32_t const style, int32_t const x, int32_t const y, int32_t const width, int32_t const height, void *const parentWindow, void *const menu, void *const instance, void *const param);

    int64_t __stdcall DefWindowProcW(void *const hWnd, uint32_t const uMsg, uint64_t const wParam, int64_t const lParam);

    int32_t __stdcall DestroyWindow(void *const hWnd);

    int32_t __stdcall DispatchMessageW(MSG const*const lpMsg);

    int32_t __stdcall GetClassInfoExW(void *const hInstance, wchar_t const*const lpszClass, WNDCLASSEXW *const lpwcx);

    uint32_t __stdcall GetLastError();

    int32_t __stdcall GetMessageW(MSG *const msg, void *const hWnd, uint32_t const wMsgFilterMin, uint32_t const wMsgFilterMax);

    void* __stdcall GetModuleHandleW(wchar_t const*const moduleName);

    void* __stdcall GetStdHandle(uint32_t const nStdHandle);

    int32_t __stdcall IsWindow(void *const hWnd);

    int32_t __stdcall IsWindowVisible(void *const hWnd);

    uint16_t __stdcall RegisterClassExW(WNDCLASSEXW const*const lpwcx);

    int32_t __stdcall ShowWindow(void *const hWnd, int32_t const nCmdShow);

    int32_t __stdcall TranslateMessage(MSG const*const lpMsg);

    int32_t __stdcall UnregisterClassW(wchar_t const*const lpClassName, void *const hInstance);

    int __stdcall WriteFile(void *const hFile, void const*const lpBuffer, uint32_t const nNumberOfBytesToWrite, uint32_t *const lpNumberOfBytesWritten, void *const lpOverlapped);

    void __stdcall ExitProcess(uint32_t const uExitCode);
}}
// extern "C" namespace Aiva::WinApi
