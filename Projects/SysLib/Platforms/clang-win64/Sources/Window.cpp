#include <SysLib/Window.hpp>
#include <WinApi.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;
using namespace SysLib;

namespace
{
    constexpr const uintptr_t INVALID_WINDOW_HANDLE{};
    uintptr_t GNumActiveWindows{};

    void InitWindowSystem();
    void ShutWindowSystem();

    uintptr_t CreateWindow(uintptr_t const width, uintptr_t const height);
    void CenterWindow(uintptr_t const windowHandle);
    void RemoveWindow(uintptr_t const windowHandle);
}
// namespace

Window::Window(uintptr_t const width, uintptr_t const height)
{
    if (!GNumActiveWindows)
        InitWindowSystem();

    m_handle = CreateWindow(width, height);
    if (!m_handle)
        return;

    CenterWindow(m_handle);
    GNumActiveWindows++;
}

Window::~Window()
{
    if (!m_handle)
        return;

    RemoveWindow(m_handle);
    m_handle = INVALID_WINDOW_HANDLE;

    if (!GNumActiveWindows)
        return;

    GNumActiveWindows--;

    if (!GNumActiveWindows)
        ShutWindowSystem();
}

Window::operator bool() const
{
	return m_handle;
}

namespace
{
    void InitWindowSystem()
    {
        WNDCLASSEXW windowClass{};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.lpfnWndProc = DefWindowProcW;
        windowClass.hInstance = GetModuleHandleW(nullptr);
        windowClass.lpszClassName = L"Aiva::SysLib::Window";

        RegisterClassExW(&windowClass);
    }

    void ShutWindowSystem()
    {
        UnregisterClassW(L"Aiva::SysLib::Window", GetModuleHandleW(nullptr));
    }

    uintptr_t CreateWindow(uintptr_t const width, uintptr_t const height)
    {
        return (uintptr_t)CreateWindowExW
        (
            /*dwExStyle*/ {},
            /*lpClassName*/ L"Aiva::SysLib::Window",
            /*lpWindowName*/ L"Window",
            /*dwStyle*/ WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            /*X*/ 0,
            /*Y*/ 0,
            /*nWidth*/ width,
            /*nHeight*/ height,
            /*hWndParent*/ nullptr,
            /*hMenu*/ nullptr,
            /*hInstance*/ GetModuleHandleW(nullptr),
            /*lpParam*/ nullptr
        );
    }

    void CenterWindow(uintptr_t const windowHandle)
    {
        if (!IsWindow((void*)windowHandle))
            return;

        auto windowRect = RECT{};
        if (!GetWindowRect((void*)windowHandle, &windowRect))
            return;

        auto const monitor = MonitorFromWindow((void*)windowHandle, MONITOR_DEFAULTTONEAREST);
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
            /*hWnd*/ (void*)windowHandle,
            /*hWndInsertAfter*/ HWND_TOP,
            /*X*/ posX,
            /*Y*/ posY,
            /*cx*/ 0,
            /*cy*/ 0,
            /*uFlags*/ SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE
        );
    }

    void RemoveWindow(uintptr_t const windowHandle)
    {
        if (!IsWindow((void*)windowHandle))
            return;

        DestroyWindow((void*)windowHandle);
    }
}
