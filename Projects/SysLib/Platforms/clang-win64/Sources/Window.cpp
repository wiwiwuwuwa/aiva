#include <SysLib/Window.hpp>
#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;
using namespace SysLib;

namespace
{
    constexpr const uintptr_t INVALID_WINDOW_HANDLE = 0;
    constexpr const uintptr_t MAIN_WINDOW_HANDLE = 1;

    void* GMainWindowHandle = nullptr;

    void InitMainWindow(uintptr_t const width, uintptr_t const height);
    void CenterMainWindow();
    void ShutMainWindow();
}
// namespace

Window::Window(uintptr_t const width, uintptr_t const height)
{
    if (IsWindow(GMainWindowHandle))
        { m_handle = INVALID_WINDOW_HANDLE; return; }

    InitMainWindow(width, height);

    if (IsWindow(GMainWindowHandle))
        { m_handle = MAIN_WINDOW_HANDLE; return; }

    m_handle = INVALID_WINDOW_HANDLE;
}

Window::~Window()
{
    if (m_handle == INVALID_WINDOW_HANDLE)
        return;

    ShutMainWindow();
}

Window::operator bool() const
{
	return m_handle;
}

namespace
{
    void InitMainWindow(uintptr_t const width, uintptr_t const height)
    {
        auto tmp = WNDCLASSEXW{};

        if (!GetClassInfoExW(GetModuleHandleW(nullptr), L"Aiva::SysLib::Window", &tmp))
        {
            WNDCLASSEXW windowClass{};
            windowClass.cbSize = sizeof(WNDCLASSEXW);
            windowClass.lpfnWndProc = DefWindowProcW;
            windowClass.hInstance = GetModuleHandleW(nullptr);
            windowClass.lpszClassName = L"Aiva::SysLib::Window";

            if (RegisterClassExW(&windowClass) == 0)
                return;
        }

        if (!IsWindow(GMainWindowHandle))
        {
            GMainWindowHandle = CreateWindowExW
            (
                /*dwExStyle*/ {},
                /*lpClassName*/ L"Aiva::SysLib::Window",
                /*lpWindowName*/ L"Window",
                /*dwStyle*/ WS_OVERLAPPEDWINDOW,
                /*X*/ 0,
                /*Y*/ 0,
                /*nWidth*/ width,
                /*nHeight*/ height,
                /*hWndParent*/ nullptr,
                /*hMenu*/ nullptr,
                /*hInstance*/ GetModuleHandleW(nullptr),
                /*lpParam*/ nullptr
            );

            if (GMainWindowHandle == nullptr)
                return;
        }

        if (!IsWindowVisible(GMainWindowHandle))
        {
            const auto result = ShowWindow(GMainWindowHandle, SW_SHOWNORMAL);

            if (result != 0)
                return;
        }

        CenterMainWindow();
    }

    void CenterMainWindow()
    {
        if (!IsWindow(GMainWindowHandle))
            return;

        auto windowRect = RECT{};
        if (!GetWindowRect(GMainWindowHandle, &windowRect))
            return;

        auto const monitor = MonitorFromWindow(GMainWindowHandle, MONITOR_DEFAULTTONEAREST);
        if (!monitor)
            return;

        auto monitorInfo = MONITORINFO{};
        monitorInfo.cbSize = sizeof(MONITORINFO);
        if (!GetMonitorInfoW(monitor, &monitorInfo))
            return;

        auto const posX = (monitorInfo.rcWork.right - monitorInfo.rcWork.left) / 2 - (windowRect.right - windowRect.left) / 2;
        auto const posY = (monitorInfo.rcWork.bottom - monitorInfo.rcWork.top) / 2 - (windowRect.bottom - windowRect.top) / 2;

        SetWindowPos
        (
            /*hWnd*/ GMainWindowHandle,
            /*hWndInsertAfter*/ HWND_TOP,
            /*X*/ posX,
            /*Y*/ posY,
            /*cx*/ 0,
            /*cy*/ 0,
            /*uFlags*/ SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE
        );
    }

    void ShutMainWindow()
    {
        auto tmp = WNDCLASSEXW{};

        if (IsWindowVisible(GMainWindowHandle))
            ShowWindow(GMainWindowHandle, SW_HIDE);

        if (IsWindow(GMainWindowHandle))
            DestroyWindow(GMainWindowHandle);

        if (GetClassInfoExW(GetModuleHandleW(nullptr), L"Aiva::SysLib::Window", &tmp))
            UnregisterClassW(L"Aiva::SysLib::Window", GetModuleHandleW(nullptr));
    }
}
