#include "Windows.hpp"

#include "Coroutines.hpp"
#include "Ensures.hpp"
#include "Intrin.hpp"
#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Windows;


namespace
{
    class WindowClass final : public NonCopyable
    {
    public:
        WindowClass();
        ~WindowClass();
    };


    class Window final : public IWindow, public NonCopyable
    {
    public:
        Window();
        ~Window() override;

    private:
        void WindowRoutine(Coroutines::IControl const& control);

        volatile uintptr_t m_stopFlag;
        volatile uintptr_t m_stopInfo;
        void* m_handle;
    };


    class System final : public NonCopyable
    {
    public:
        System();
        ~System();

        IWindow const& GetWindow() const;

    private:
        ManageObject<WindowClass> m_windowClass;
        ManageObject<Window> m_window;
        ManageObject<Window> m_window1;
        ManageObject<Window> m_window2;
        ManageObject<Window> m_window3;
    };


    SpinLock GSystemLock;
    ManageObject<System> GSystem;
}
// namespace


WindowClass::WindowClass()
{
    auto windowClass = WinApi::WNDCLASSEXW{};
    windowClass.cbSize = sizeof(WinApi::WNDCLASSEXW);
    windowClass.lpfnWndProc = WinApi::DefWindowProcW;
    windowClass.hInstance = WinApi::GetModuleHandleW(nullptr);
    windowClass.lpszClassName = L"Aiva::Window";

    if (WinApi::RegisterClassExW(&windowClass) == 0)
        CheckNoEntry();
}


WindowClass::~WindowClass()
{
    auto const lpszClassName = L"Aiva::Window";
    auto const hInstance = WinApi::GetModuleHandleW(nullptr);

    if (!WinApi::UnregisterClassW(lpszClassName, hInstance))
        CheckNoEntry();
}


Window::Window()
{
    Intrin::AtomicExchange(&m_stopFlag, 0);
    Intrin::AtomicExchange(&m_stopInfo, 0);

    Coroutines::Spawn([this](Coroutines::IControl const& control)
        { WindowRoutine(control); });
}


Window::~Window()
{
    Intrin::AtomicExchange(&m_stopFlag, 1);

    while (Intrin::AtomicCompareExchange(&m_stopInfo, 0, 0) != 1)
        Intrin::YieldProcessor();
}


void Window::WindowRoutine(Coroutines::IControl const& control)
{
    auto const pinnedWorker = control.GetCurrWorker();
    if (!pinnedWorker)
        CheckNoEntry();

    m_handle = WinApi::CreateWindowExW
    (
        /*dwExStyle*/ 0,
        /*lpClassName*/ L"Aiva::Window",
        /*lpWindowName*/ L"Window",
        /*dwStyle*/ WinApi::WS_OVERLAPPEDWINDOW | WinApi::WS_VISIBLE,
        /*X*/ 0,
        /*Y*/ 0,
        /*nWidth*/ 1280,
        /*nHeight*/ 720,
        /*hWndParent*/ nullptr,
        /*hMenu*/ nullptr,
        /*hInstance*/ WinApi::GetModuleHandleW(nullptr),
        /*lpParam*/ nullptr
    );
    if (!m_handle)
        CheckNoEntry();

    auto windowRect = WinApi::RECT{};
    if (!WinApi::GetWindowRect(m_handle, &windowRect))
        CheckNoEntry();

    auto const monitor = WinApi::MonitorFromWindow(m_handle, WinApi::MONITOR_DEFAULTTONEAREST);
    if (!monitor)
        CheckNoEntry();

    auto monitorInfo = WinApi::MONITORINFO{};
    monitorInfo.cbSize = sizeof(WinApi::MONITORINFO);
    if (!WinApi::GetMonitorInfoW(monitor, &monitorInfo))
        CheckNoEntry();

    auto const posX = (monitorInfo.rcWork.right - monitorInfo.rcWork.left) / 2 - (windowRect.right - windowRect.left) / 2;
    auto const posY = (monitorInfo.rcWork.bottom - monitorInfo.rcWork.top) / 2 - (windowRect.bottom - windowRect.top) / 2;

    if (!WinApi::SetWindowPos
    (
        /* hWnd */ m_handle,
        /* hWndInsertAfter */ WinApi::HWND_TOP,
        /* X */ posX,
        /* Y */ posY,
        /* cx */ 0,
        /* cy */ 0,
        /* uFlags */ WinApi::SWP_NOZORDER | WinApi::SWP_NOSIZE | WinApi::SWP_NOACTIVATE
    ))
        CheckNoEntry();

    while (Intrin::AtomicCompareExchange(&m_stopFlag, 0, 0) == 0)
    {
        auto msg = WinApi::MSG{};

        while (WinApi::PeekMessageW(&msg, m_handle, 0, 0, WinApi::PM_REMOVE))
        {
            WinApi::TranslateMessage(&msg);
            WinApi::DispatchMessageW(&msg);
        }

        control.Yield(pinnedWorker);
    }

    if (!WinApi::DestroyWindow(m_handle))
        CheckNoEntry();
    m_handle = nullptr;

    Intrin::AtomicExchange(&m_stopInfo, 1);
}


System::System()
{
    m_windowClass.Construct();
    m_window.Construct();

    m_window1.Construct();
    m_window2.Construct();
    m_window3.Construct();
}


System::~System()
{
    m_window3.Destruct();
    m_window2.Destruct();
    m_window1.Destruct();

    m_window.Destruct();
    m_windowClass.Destruct();
}


IWindow const& System::GetWindow() const
{
    return m_window.GetObjectRef();
}


void Windows::InitSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (GSystem)
        CheckNoEntry();

    GSystem.Construct();
}


void Windows::ShutSystem()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    GSystem.Destruct();
}


IWindow const& Windows::GetMainWindow()
{
    SpinLockScope_t const lockScope{ GSystemLock };

    if (!GSystem)
        CheckNoEntry();

    return GSystem->GetWindow();
}
