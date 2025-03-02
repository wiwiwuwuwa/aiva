#include <SysLib/Window.hpp>
#include <WinApi.hpp>
#include <SysLib/Console.hpp>

using namespace Aiva;
using namespace MinLib;
using namespace WinApi;
using namespace SysLib;

namespace
{
    constexpr const uint32_t INVALID_WINDOW_HANDLE = 0;
    constexpr const uint32_t MAIN_WINDOW_HANDLE = 1;

    void* GMainWindowHandle = nullptr;

    void InitMainWindow(uint32_t const width, uint32_t const height);
    void ShutMainWindow();
}
// namespace

Window::Window(uint32_t const width, uint32_t const height)
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
    void InitMainWindow(uint32_t const width, uint32_t const height)
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
                /*lpWindowName*/ L"Aiva Window",
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
