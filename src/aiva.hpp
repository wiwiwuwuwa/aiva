#pragma once
// です。


// ------------------------------------
// "basic_templates.hpp"


namespace Aiva::Templates
{
    template <typename...>
    using Void_t = void;


    template <typename TType>
    struct TypeIdentity { using Type_t = TType; };


    template <typename TType>
    using TypeIdentity_t = typename TypeIdentity<TType>::Type_t;


    template <typename TType>
    struct RemoveReference : public TypeIdentity<TType> {};


    template <typename TType>
    struct RemoveReference<TType&> : public TypeIdentity<TType> {};


    template <typename TType>
    struct RemoveReference<TType&&> : public TypeIdentity<TType> {};


    template <typename TType>
    using RemoveReference_t = typename RemoveReference<TType>::Type_t;


    template <typename TType>
    constexpr TType&& Forward(TType& value);


    template <typename TType>
    constexpr TType&& Forward(TType&& value);


    template <typename TType>
    constexpr RemoveReference_t<TType>&& Move(TType&& value);
}


// ------------------------------------
// "basic_templates.inl"


namespace Aiva::Templates
{
    template <typename TType>
    constexpr TType&& Forward(TType& value)
    {
        return static_cast<TType&&>(value);
    }


    template <typename TType>
    constexpr TType&& Forward(TType&& value)
    {
        return static_cast<TType&&>(value);
    }


    template <typename TType>
    constexpr RemoveReference_t<TType>&& Move(TType&& value)
    {
        return static_cast<RemoveReference_t<TType>&&>(value);
    }
}


// ------------------------------------
// "number_templates.hpp"


namespace Aiva::Templates
{
    enum class NumberType
    {
        Int,
        Flt,
        MAX,
    };


    enum class NumberSign
    {
        None,
        Sign,
        MAX,
    };


    enum class NumberSize
    {
        Byte1,
        Byte2,
        Byte4,
        Byte8,
        MAX,
    };


    template <typename TType, typename = void>
    struct IsNumber final { constexpr operator bool() const { return false; } };


    template <typename TType>
    struct IsNumber<TType, Void_t<decltype(TType{ 3 } / TType{ 2 })>> final { constexpr operator bool() const { return true; } };


    template <typename TType>
    constexpr NumberType GetNumberType();


    template <typename TType>
    constexpr NumberSign GetNumberSign();


    template <typename TType>
    constexpr NumberSize GetNumberSize();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber();


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    using Number_t = decltype(CreateNumber<TType, TSign, TSize>());
}


// ------------------------------------
// "number_templates.inl"


namespace Aiva::Templates
{
    template <typename TType>
    constexpr NumberType GetNumberType()
    {
        if constexpr (IsNumber<TType>())
        {
            if constexpr (TType{ 3 } / TType{ 2 } == TType{ 1 })
                { return NumberType::Int; }

            else
                { return NumberType::Flt; }
        }
        else
            { return NumberType::MAX; }
    }


    template <typename TType>
    constexpr NumberSign GetNumberSign()
    {
        if constexpr (IsNumber<TType>())
        {
            if constexpr ((TType)(-1) < (TType)(0))
                { return NumberSign::Sign; }

            else
                { return NumberSign::None; }
        }
        else
            { return NumberSign::MAX; }
    }


    template <typename TType>
    constexpr NumberSize GetNumberSize()
    {
        if constexpr (sizeof(TType) == 1)
            { return NumberSize::Byte1; }

        else if constexpr (sizeof(TType) == 2)
            { return NumberSize::Byte2; }

        else if constexpr (sizeof(TType) == 4)
            { return NumberSize::Byte4; }

        else if constexpr (sizeof(TType) == 8)
            { return NumberSize::Byte8; }

        else
            { return NumberSize::MAX; }
    }


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber()
    {
        #define CreateNumber_Begin() \
            if constexpr (TType == NumberType::MAX || TSign == NumberSign::MAX || TSize == NumberSize::MAX) \
                { return nullptr; } \

        #define CreateNumber_CheckBaseType(type) \
            else if constexpr (TType == GetNumberType<type>() && TSign == GetNumberSign<type>() && TSize == GetNumberSize<type>()) \
                { type constexpr const zero{}; return zero; } \

        #define CreateNumber_End() \
            else \
                { return nullptr; } \

        CreateNumber_Begin()
        CreateNumber_CheckBaseType(char)
        CreateNumber_CheckBaseType(signed char)
        CreateNumber_CheckBaseType(unsigned char)
        CreateNumber_CheckBaseType(short int)
        CreateNumber_CheckBaseType(unsigned short int)
        CreateNumber_CheckBaseType(int)
        CreateNumber_CheckBaseType(unsigned int)
        CreateNumber_CheckBaseType(long int)
        CreateNumber_CheckBaseType(unsigned long int)
        CreateNumber_CheckBaseType(long long int)
        CreateNumber_CheckBaseType(unsigned long long int)
        CreateNumber_CheckBaseType(float)
        CreateNumber_CheckBaseType(double)
        CreateNumber_CheckBaseType(long double)
        CreateNumber_End()

        #undef CreateNumber_End
        #undef CreateNumber_CheckBaseType
        #undef CreateNumber_Begin
    }
}


// ------------------------------------
// "primitive_types.hpp"


namespace Aiva
{
    #define DefineNumber_Classic(name, type, sign, size) \
        using name = Templates::Number_t<Templates::NumberType::type, Templates::NumberSign::sign, Templates::NumberSize::size>; \

    #define DefineNumber_Pointer(name, type, sign) \
        using name = Templates::Number_t<Templates::NumberType::type, Templates::NumberSign::sign, Templates::GetNumberSize<void*>()>; \

    DefineNumber_Classic(sint8_t, Int, Sign, Byte1)
    DefineNumber_Classic(sint16_t, Int, Sign, Byte2)
    DefineNumber_Classic(sint32_t, Int, Sign, Byte4)
    DefineNumber_Classic(sint64_t, Int, Sign, Byte8)
    DefineNumber_Classic(uint8_t, Int, None, Byte1)
    DefineNumber_Classic(uint16_t, Int, None, Byte2)
    DefineNumber_Classic(uint32_t, Int, None, Byte4)
    DefineNumber_Classic(uint64_t, Int, None, Byte8)
    DefineNumber_Classic(float32_t, Flt, None, Byte4)
    DefineNumber_Classic(float64_t, Flt, None, Byte8)
    DefineNumber_Pointer(sintptr_t, Int, Sign)
    DefineNumber_Pointer(uintptr_t, Int, None)

    #undef DefineNumber_Pointer
    #undef DefineNumber_Classic

    enum class byte_t : uint8_t {};
    using size_t = decltype(sizeof(byte_t));
    using nullptr_t = decltype(nullptr);
}


// ------------------------------------
// "winapi.hpp"


extern "C" { namespace Aiva::WinApi
{
    // Base Types

    using BYTE = uint8_t;
    using WORD = uint16_t;
    using DWORD = uint32_t;
    using UINT = uint32_t;
    using LONG = sint32_t;
    using ULONG = uint32_t;
    using BOOL = sint32_t;
    using ATOM = WORD;
    using HANDLE = void*;
    using LPVOID = void*;
    using LPCVOID = void const*;
    using LPDWORD = DWORD*;
    using LPBOOL = BOOL*;
    using SIZE_T = size_t;
    using ULONG_PTR = uintptr_t;
    using UINT_PTR = uintptr_t;
    using LONG_PTR = sintptr_t;

    using WCHAR = wchar_t;
    using LPCWSTR = WCHAR const*;
    using LPWSTR = WCHAR*;
    using HMODULE = HANDLE;
    using HINSTANCE = HANDLE;
    using HWND = HANDLE;
    using HMENU = HANDLE;
    using HMONITOR = HANDLE;

    using HRESULT = LONG;
    using LRESULT = LONG_PTR;
    using WPARAM = UINT_PTR;
    using LPARAM = LONG_PTR;

    using LPTHREAD_START_ROUTINE = DWORD(__attribute__((stdcall))*)(LPVOID);
    using LPFIBER_START_ROUTINE = void (__attribute__((stdcall))*)(LPVOID);
    using WNDPROC = LRESULT(__attribute__((stdcall))*)(HWND, UINT, WPARAM, LPARAM);

    // Structures

    struct GUID final
    {
        DWORD Data1;
        WORD Data2;
        WORD Data3;
        BYTE Data4[8];
    };


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


    struct LUID;

    // Constants

    static auto const INVALID_HANDLE_VALUE = (HANDLE)(LONG_PTR)(-1);
    static auto const STD_INPUT_HANDLE = (DWORD)(-10);
    static auto const STD_OUTPUT_HANDLE = (DWORD)(-11);
    static auto const STD_ERROR_HANDLE = (DWORD)(-12);
    static auto const INFINITE = (DWORD)(-1);
    static auto const WAIT_FAILED = (DWORD)(-1);

    static auto const WS_OVERLAPPED = (DWORD)(0x00000000L);
    static auto const WS_CAPTION = (DWORD)(0x00C00000L);
    static auto const WS_SYSMENU = (DWORD)(0x00080000L);
    static auto const WS_THICKFRAME = (DWORD)(0x00040000L);
    static auto const WS_MINIMIZEBOX = (DWORD)(0x00020000L);
    static auto const WS_MAXIMIZEBOX = (DWORD)(0x00010000L);
    static auto const WS_OVERLAPPEDWINDOW = (DWORD)(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

    static auto const PM_REMOVE = (UINT)(0x0001);

    static auto const MONITOR_DEFAULTTONEAREST = (DWORD)(0x00000002);
    static auto const HWND_TOP = (HANDLE)(0);
    static auto const SWP_NOSIZE = (UINT)(0x0001);
    static auto const SWP_NOZORDER = (UINT)(0x0004);
    static auto const SWP_SHOWWINDOW = (UINT)(0x0040);

    // Process Management

    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT uExitCode);

    // Heap Management

    __attribute__((dllimport, stdcall)) HANDLE HeapCreate(DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
    __attribute__((dllimport, stdcall)) BOOL HeapDestroy(HANDLE hHeap);
    __attribute__((dllimport, stdcall)) LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
    __attribute__((dllimport, stdcall)) BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

    // Process/Thread Management

    __attribute__((dllimport, stdcall)) HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
    __attribute__((dllimport, stdcall)) BOOL CloseHandle(HANDLE hObject);
    __attribute__((dllimport, stdcall)) DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

    // System Information

    __attribute__((dllimport, stdcall)) void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);

    // Fiber Management

    __attribute__((dllimport, stdcall)) LPVOID ConvertThreadToFiber(LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) BOOL IsThreadAFiber();
    __attribute__((dllimport, stdcall)) BOOL ConvertFiberToThread(void);
    __attribute__((dllimport, stdcall)) LPVOID CreateFiber(SIZE_T dwStackSize, LPFIBER_START_ROUTINE lpStartAddress, LPVOID lpParameter);
    __attribute__((dllimport, stdcall)) void SwitchToFiber(LPVOID lpFiber);
    __attribute__((dllimport, stdcall)) void DeleteFiber(LPVOID lpFiber);

    // File Management

    __attribute__((dllimport, stdcall)) HANDLE GetStdHandle(DWORD nStdHandle);
    __attribute__((dllimport, stdcall)) BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

    // Module/Instance Management

    __attribute__((dllimport, stdcall)) HMODULE GetModuleHandleW(LPCWSTR lpModuleName);

    // Window Class Management

    __attribute__((dllimport, stdcall)) ATOM RegisterClassExW(const WNDCLASSEXW *lpwcx);
    __attribute__((dllimport, stdcall)) BOOL UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance);
    __attribute__((dllimport, stdcall)) LRESULT DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    // Window Management

    __attribute__((dllimport, stdcall)) HWND CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
    __attribute__((dllimport, stdcall)) BOOL DestroyWindow(HWND hWnd);
    __attribute__((dllimport, stdcall)) BOOL IsWindow(HWND hWnd);
    __attribute__((dllimport, stdcall)) BOOL GetWindowRect(HWND hWnd, LPRECT lpRect);
    __attribute__((dllimport, stdcall)) BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

    // Message Handling

    __attribute__((dllimport, stdcall)) BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
    __attribute__((dllimport, stdcall)) BOOL TranslateMessage(const MSG *lpMsg);
    __attribute__((dllimport, stdcall)) LRESULT DispatchMessageW(const MSG *lpMsg);

    // Display/Monitor Management

    __attribute__((dllimport, stdcall)) HMONITOR MonitorFromWindow(HWND hwnd, DWORD dwFlags);
    __attribute__((dllimport, stdcall)) BOOL GetMonitorInfoW(HMONITOR hMonitor, LPMONITORINFO lpmi);
}}


// ------------------------------------
// "wincom.hpp"


namespace Aiva::WinCom
{
    struct IUnknown
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT QueryInterface(WinApi::GUID const& riid, void** ppvObject) = 0;
        __attribute__((stdcall)) virtual WinApi::ULONG AddRef() = 0;
        __attribute__((stdcall)) virtual WinApi::ULONG Release() = 0;
    };


    extern "C" WinApi::GUID const IID_IUnknown;
}


// ------------------------------------
// "dxgi.hpp"


namespace Aiva::Dxgi
{
    struct DXGI_SWAP_CHAIN_DESC;
    struct DXGI_SWAP_CHAIN_DESC1;
    struct DXGI_SWAP_CHAIN_FULLSCREEN_DESC;
    struct IDXGIAdapter;
    struct IDXGIAdapter1;
    struct IDXGIOutput;
    struct IDXGISwapChain;
    struct IDXGISwapChain1;


    enum class DXGI_FEATURE
    {
        DXGI_FEATURE_PRESENT_ALLOW_TEARING = 0
    };


    enum class DXGI_GPU_PREFERENCE
    {
        DXGI_GPU_PREFERENCE_UNSPECIFIED = 0,
        DXGI_GPU_PREFERENCE_MINIMUM_POWER = DXGI_GPU_PREFERENCE_UNSPECIFIED + 1,
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE = DXGI_GPU_PREFERENCE_MINIMUM_POWER + 1,
    };


    struct IDXGIObject : public WinCom::IUnknown
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT SetPrivateData(WinApi::GUID const& Name, WinApi::UINT const DataSize, void const*const pData) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT SetPrivateDataInterface(WinApi::GUID const& Name, IUnknown const*const pUnknown) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT GetPrivateData(WinApi::GUID const& Name, WinApi::UINT *const pDataSize, void *const pData) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT GetParent(WinApi::GUID const& riid, void **const ppParent) = 0;
    };


    struct IDXGIFactory : public IDXGIObject
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT EnumAdapters(WinApi::UINT const Adapter, IDXGIAdapter **const ppAdapter) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT MakeWindowAssociation(WinApi::HWND const WindowHandle, WinApi::UINT const Flags) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT GetWindowAssociation(WinApi::HWND *const pWindowHandle) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT CreateSwapChain(IUnknown const*const pDevice, DXGI_SWAP_CHAIN_DESC const*const pDesc, IDXGISwapChain **const ppSwapChain) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT CreateSoftwareAdapter(WinApi::HMODULE const Module, IDXGIAdapter **const ppAdapter) = 0;
    };


    struct IDXGIFactory1 : public IDXGIFactory
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT EnumAdapters1(WinApi::UINT const Adapter, IDXGIAdapter1 **const ppAdapter) = 0;
        __attribute__((stdcall)) virtual WinApi::BOOL IsCurrent() = 0;
    };


    struct IDXGIFactory2 : public IDXGIFactory1
    {
        __attribute__((stdcall)) virtual WinApi::BOOL IsWindowedStereoEnabled() = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT CreateSwapChainForHwnd(IUnknown *const pDevice, WinApi::HWND const hWnd, DXGI_SWAP_CHAIN_DESC1 const*const pDesc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const*const pFullscreenDesc, IDXGIOutput *const pRestrictToOutput, IDXGISwapChain1 **const ppSwapChain) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT CreateSwapChainForCoreWindow(IUnknown *const pDevice, IUnknown *const pWindow, DXGI_SWAP_CHAIN_DESC1 const*const pDesc, IDXGIOutput *const pRestrictToOutput, IDXGISwapChain1 **const ppSwapChain) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT GetSharedResourceAdapterLuid(WinApi::HANDLE const hResource, WinApi::LUID *const pLuid) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT RegisterStereoStatusWindow(WinApi::HWND const WindowHandle, WinApi::UINT const wMsg, WinApi::DWORD *const pdwCookie) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT RegisterStereoStatusEvent(WinApi::HANDLE const hEvent, WinApi::DWORD *const pdwCookie) = 0;
        __attribute__((stdcall)) virtual void UnregisterStereoStatus(WinApi::DWORD const dwCookie) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT RegisterOcclusionStatusWindow(WinApi::HWND const WindowHandle, WinApi::UINT const wMsg, WinApi::DWORD *const pdwCookie) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT RegisterOcclusionStatusEvent(WinApi::HANDLE const hEvent, WinApi::DWORD *const pdwCookie) = 0;
        __attribute__((stdcall)) virtual void UnregisterOcclusionStatus(WinApi::DWORD const dwCookie) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT CreateSwapChainForComposition(IUnknown *const pDevice, DXGI_SWAP_CHAIN_DESC1 const*const pDesc, IDXGIOutput *const pRestrictToOutput, IDXGISwapChain1 **const ppSwapChain) = 0;
    };


    struct IDXGIFactory3 : public IDXGIFactory2
    {
        __attribute__((stdcall)) virtual WinApi::UINT GetCreationFlags() = 0;
    };


    struct IDXGIFactory4 : public IDXGIFactory3
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT EnumAdapterByLuid(WinApi::LUID const AdapterLuid, WinApi::GUID const& riid, void **const ppvAdapter) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT EnumWarpAdapter(WinApi::GUID const& riid, void **const ppvAdapter) = 0;
    };


    struct IDXGIFactory5 : public IDXGIFactory4
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT CheckFeatureSupport(DXGI_FEATURE const Feature, void *const pFeatureSupportData, WinApi::UINT const FeatureSupportDataSize) = 0;
    };


    struct IDXGIFactory6 : public IDXGIFactory5
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT EnumAdapterByGpuPreference(WinApi::UINT const Adapter, DXGI_GPU_PREFERENCE const GpuPreference, WinApi::GUID const& riid, void **const ppvAdapter) = 0;
    };


    struct IDXGIFactory7 : public IDXGIFactory6
    {
        __attribute__((stdcall)) virtual WinApi::HRESULT RegisterAdaptersChangedEvent(WinApi::HANDLE const hEvent, WinApi::DWORD *const pdwCookie) = 0;
        __attribute__((stdcall)) virtual WinApi::HRESULT UnregisterAdaptersChangedEvent(WinApi::DWORD const dwCookie) = 0;
    };


    static inline constexpr WinApi::UINT const DXGI_CREATE_FACTORY_DEBUG{ 0x01 };


    extern "C" WinApi::GUID const IID_IDXGIObject;
    extern "C" WinApi::GUID const IID_IDXGIFactory;
    extern "C" WinApi::GUID const IID_IDXGIFactory1;
    extern "C" WinApi::GUID const IID_IDXGIFactory2;
    extern "C" WinApi::GUID const IID_IDXGIFactory3;
    extern "C" WinApi::GUID const IID_IDXGIFactory4;
    extern "C" WinApi::GUID const IID_IDXGIFactory5;
    extern "C" WinApi::GUID const IID_IDXGIFactory6;
    extern "C" WinApi::GUID const IID_IDXGIFactory7;
    extern "C" WinApi::GUID const IID_IDXGIAdapter;
    extern "C" WinApi::GUID const IID_IDXGIAdapter1;
    extern "C" WinApi::GUID const IID_IDXGIOutput;
    extern "C" WinApi::GUID const IID_IDXGISwapChain;
    extern "C" WinApi::GUID const IID_IDXGISwapChain1;


    extern "C" __attribute__((dllimport, stdcall)) WinApi::HRESULT CreateDXGIFactory2(WinApi::UINT const Flags, WinApi::GUID const& riid, void **const ppFactory);
}


// ------------------------------------
// "intrin.hpp"


namespace Aiva::Intrin
{
    inline uintptr_t AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange);
    inline uintptr_t AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange);

    inline void YieldProcessor();
}


// ------------------------------------
// "intrin.inl"


namespace Aiva::Intrin
{
    inline uintptr_t AtomicCompareExchange(volatile uintptr_t *const Destination, uintptr_t const Comperand, uintptr_t const Exchange)
    {
        auto comperand = Comperand;
        auto exchange = Exchange;
        __atomic_compare_exchange(Destination, &comperand, &exchange, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

        return comperand;
    }


    inline uintptr_t AtomicExchange(volatile uintptr_t *const Destination, uintptr_t const Exchange)
    {
        auto exchange = Exchange;
        auto previous = uintptr_t{};
        __atomic_exchange(Destination, &exchange, &previous, __ATOMIC_SEQ_CST);

        return previous;
    }


    inline void YieldProcessor()
    {
        __builtin_ia32_pause();
    }
}


// ------------------------------------
// "math.hpp"


namespace Aiva::Math
{
    template <typename TType>
    constexpr TType Min(TType const a, TType const b);


    template <typename TType>
    constexpr size_t CountOfBits();
}


// ------------------------------------
// "math.inl"


namespace Aiva::Math
{
    template <typename TType>
    constexpr TType Min(TType const a, TType const b)
    {
        return a <= b ? a : b;
    }


    template <typename TType>
    constexpr size_t CountOfBits()
    {
        using SameSizeUnsignedType_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::GetNumberSize<TType>()>;

        auto val = (SameSizeUnsignedType_t)(-1);
        auto cnt = size_t{};

        while (val)
        {
            val >>= 1;
            cnt++;
        }

        return cnt;
    }
}


// ------------------------------------
// "system.hpp"


namespace Aiva::System
{
    size_t GetNumberOfCores();

    [[noreturn]] void ExitSuccess();
    [[noreturn]] void ExitFailure();
}


// ------------------------------------
// "system.inl"


namespace Aiva::System
{
    size_t GetNumberOfCores()
    {
        auto systemInfo = WinApi::SYSTEM_INFO{};
        WinApi::GetSystemInfo(&systemInfo);

        return Math::Min(Math::CountOfBits<uintptr_t>(), (size_t)systemInfo.dwNumberOfProcessors);
    }


    [[noreturn]] void ExitSuccess()
    {
        WinApi::ExitProcess(0);
    }


    [[noreturn]] void ExitFailure()
    {
        WinApi::ExitProcess(1);
    }
}


// ------------------------------------
// "cstr_view.hpp"


namespace Aiva
{
    class CstrView final
    {
    public:
        constexpr CstrView();
        constexpr CstrView(char const& data);
        constexpr CstrView(char const*const data);
        constexpr operator char const&() const;
        constexpr operator char const*() const;

        constexpr operator bool() const;
        constexpr size_t StrLen() const;
        constexpr char operator[](size_t const index) const;
        constexpr char const& GetDataRef() const;
        constexpr char const* GetDataPtr() const;

    private:
        char const& m_data;
    };


    constexpr size_t StrLen(CstrView const str);
}


// ------------------------------------
// "cstr_view.inl"


namespace Aiva
{
    constexpr CstrView::CstrView() : m_data{ *"" }
    {
        //
    }


    constexpr CstrView::CstrView(char const& data) : m_data{ data }
    {
        //
    }


    constexpr CstrView::CstrView(char const*const data) : m_data{ data ? *data : *"" }
    {
        //
    }


    constexpr CstrView::operator char const&() const
    {
        return m_data;
    }


    constexpr CstrView::operator char const*() const
    {
        return &m_data;
    }


    constexpr CstrView::operator bool() const
    {
        return (&m_data)[0] != '\0';
    }


    constexpr size_t CstrView::StrLen() const
    {
        auto length = size_t{};

        while ((&m_data)[length] != '\0')
            length++;

        return length;
    }


    constexpr char CstrView::operator[](size_t const index) const
    {
        return (&m_data)[index];
    }


    constexpr size_t StrLen(CstrView const str)
    {
        return str.StrLen();
    }


    constexpr char const& CstrView::GetDataRef() const
    {
        return m_data;
    }


    constexpr char const* CstrView::GetDataPtr() const
    {
        return &m_data;
    }
}


// ------------------------------------
// "span.hpp"


namespace Aiva
{
    template <typename TType>
    class Span final
    {
    public:
        constexpr Span();
        constexpr Span(size_t const size, TType& data);
        constexpr Span(size_t const size, TType *const data);
        constexpr Span(TType& data);
        constexpr Span(TType *const data);
        constexpr Span(nullptr_t const);

        constexpr operator bool() const;
        constexpr size_t GetSize() const;
        constexpr TType& operator[](size_t const index) const;
        constexpr TType& GetDataRef() const;
        constexpr TType* GetDataPtr() const;

    private:
        size_t m_size;
        TType* m_data;
    };


    template <typename TDst, typename TSrc>
    constexpr Span<TDst> CastSpan(Span<TSrc> const& span);
}


// ------------------------------------
// "span.inl"


namespace Aiva
{
    template <typename TType>
    constexpr Span<TType>::Span()
    {
        m_size = 0;
        m_data = nullptr;
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType& data)
    {
        if (size > 0)
        {
            m_size = size;
            m_data = &data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType *const data)
    {
        if (size > 0 && data)
        {
            m_size = size;
            m_data = data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType& data)
    {
        m_size = 1;
        m_data = &data;
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType *const data)
    {
        if (data)
        {
            m_size = 1;
            m_data = data;
        }
        else
        {
            m_size = 0;
            m_data = nullptr;
        }
    }


    template <typename TType>
    constexpr Span<TType>::Span(nullptr_t const)
    {
        m_size = 0;
        m_data = nullptr;
    }


    template <typename TType>
    constexpr Span<TType>::operator bool() const
    {
        return m_size > 0 && m_data;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const
    {
        return m_size;
    }


    template <typename TType>
    constexpr TType& Span<TType>::operator[](size_t const index) const
    {
        return m_data[index];
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetDataRef() const
    {
        return *m_data;
    }


    template <typename TType>
    constexpr TType* Span<TType>::GetDataPtr() const
    {
        return m_data;
    }


    template <typename TDst, typename TSrc>
    constexpr Span<TDst> CastSpan(Span<TSrc> const& span)
    {
        if (!span)
            return Span<TDst>{};

        auto const bytesCount = sizeof(TSrc) * span.GetSize();
        if (bytesCount % sizeof(TDst))
            return Span<TDst>{};

        auto const dataAddr = reinterpret_cast<uintptr_t>(span.GetDataPtr());
        if (dataAddr % alignof(TDst))
            return Span<TDst>{};

        auto& data = reinterpret_cast<TDst&>(span.GetDataRef());
        auto const size = bytesCount / sizeof(TDst);

        return Span<TDst>{ size, data };
    }
}


// ------------------------------------
// "non_copyable.hpp"


namespace Aiva
{
    class NonCopyable
    {
    public:
        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator=(NonCopyable const&) = delete;
        NonCopyable(NonCopyable&&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;

    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    };
}


// ------------------------------------
// "lock_scope.hpp"


namespace Aiva
{
    template <typename TType>
    class LockScope : public NonCopyable
    {
    public:
        LockScope(TType const& lock);
        ~LockScope();

    private:
        TType& m_lock;
    };
}


// ------------------------------------
// "lock_scope.inl"


namespace Aiva
{
    template <typename TType>
    LockScope<TType>::LockScope(TType const& lock) : m_lock{ const_cast<TType&>(lock) }
    {
        m_lock.Lock();
    }


    template <typename TType>
    LockScope<TType>::~LockScope()
    {
        m_lock.Unlock();
    }
}


// ------------------------------------
// "spin_lock.hpp"


namespace Aiva
{
    class SpinLock final : public NonCopyable
    {
    public:
        SpinLock() = default;
        ~SpinLock() = default;

        void Lock();
        void Unlock();

    private:
        volatile uintptr_t m_locked{};
    };


    using SpinLockScope_t = LockScope<SpinLock>;
}


// ------------------------------------
// "spin_lock.inl"


namespace Aiva
{
    void SpinLock::Lock()
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange(&m_locked, 0, 1) == 0)
                break;

            Intrin::YieldProcessor();
        }
    }


    void SpinLock::Unlock()
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange(&m_locked, 1, 0) == 1)
                break;

            Intrin::YieldProcessor();
        }
    }
}


// ------------------------------------
// "new.hpp"


void* operator new(size_t const size, void *const ptr);
void* operator new[](size_t const size, void *const ptr);


// ------------------------------------
// "new.inl"


void* operator new(size_t const, void *const ptr)
{
    return ptr;
}


void* operator new[](size_t const, void *const ptr)
{
    return ptr;
}


// ------------------------------------
// "manage_object.hpp"


namespace Aiva
{
    template <typename TType>
    class ManageObject final : public NonCopyable
    {
    public:
        template <typename... TArgs>
        constexpr void Construct(TArgs&&... args);
        constexpr void Destruct();

        constexpr operator bool() const;
        constexpr TType& operator*();
        constexpr TType const& operator*() const;
        constexpr TType* operator->();
        constexpr TType const* operator->() const;

        constexpr TType& GetObjectRef();
        constexpr TType const& GetObjectRef() const;
        constexpr TType* GetObjectPtr();
        constexpr TType const* GetObjectPtr() const;

    private:
        alignas(alignof(TType)) byte_t m_object[sizeof(TType)];
        TType* m_ptr = nullptr;
    };
}


// ------------------------------------
// "manage_object.inl


namespace Aiva
{
    template <typename TType>
    template <typename... TArgs>
    constexpr void ManageObject<TType>::Construct(TArgs&&... args)
    {
        new (m_object) TType{ Templates::Forward<TArgs>(args)... };
        m_ptr = (TType*)m_object;
    }


    template <typename TType>
    constexpr void ManageObject<TType>::Destruct()
    {
        m_ptr = nullptr;
        ((TType*)m_object)->~TType();
    }


    template <typename TType>
    constexpr ManageObject<TType>::operator bool() const
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType& ManageObject<TType>::operator*()
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::operator*() const
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType* ManageObject<TType>::operator->()
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType const* ManageObject<TType>::operator->() const
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType& ManageObject<TType>::GetObjectRef()
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType const& ManageObject<TType>::GetObjectRef() const
    {
        return *m_ptr;
    }


    template <typename TType>
    constexpr TType* ManageObject<TType>::GetObjectPtr()
    {
        return m_ptr;
    }


    template <typename TType>
    constexpr TType const* ManageObject<TType>::GetObjectPtr() const
    {
        return m_ptr;
    }
}


// ------------------------------------
// "console.hpp"


namespace Aiva
{
    class Console final
    {
    public:
        static void InitSystem();
        static void ShutSystem();

        template <typename... TArgs>
        static void Print(TArgs const&... args);

        template <typename... TArgs>
        static void PrintLine(TArgs const&... args);

        template <typename... TArgs>
        static void Error(TArgs const&... args);

        template <typename... TArgs>
        static void ErrorLine(TArgs const&... args);

    private:
        Console() = delete;

        static void Print_Impl(Span<const CstrView> const& messages);
        static void Error_Impl(Span<const CstrView> const& messages);

        static SpinLock GLock;
        static bool GInitialized;
        static WinApi::HANDLE GPrintHandle;
        static WinApi::HANDLE GErrorHandle;
    };
}


// ------------------------------------
// "console.inl


namespace Aiva
{
    void Console::InitSystem()
    {
        SpinLockScope_t const lockScope{ GLock };

        if (GInitialized)
            System::ExitFailure();

        GPrintHandle = WinApi::GetStdHandle(WinApi::STD_OUTPUT_HANDLE);
        if (!GPrintHandle || GPrintHandle == WinApi::INVALID_HANDLE_VALUE)
            System::ExitFailure();

        GErrorHandle = WinApi::GetStdHandle(WinApi::STD_ERROR_HANDLE);
        if (!GErrorHandle || GErrorHandle == WinApi::INVALID_HANDLE_VALUE)
            System::ExitFailure();

        GInitialized = true;
    }


    void Console::ShutSystem()
    {
        SpinLockScope_t const lockScope{ GLock };

        if (!GInitialized)
            System::ExitFailure();

        GInitialized = false;
    }


    template <typename... TArgs>
    void Console::Print(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}... };
        Print_Impl(messages);
    }


    template <typename... TArgs>
    void Console::PrintLine(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Print_Impl(messages);
    }


    template <typename... TArgs>
    void Console::Error(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}... };
        Error_Impl(messages);
    }


    template <typename... TArgs>
    void Console::ErrorLine(TArgs const&... args)
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Error_Impl(messages);
    }


    void Console::Print_Impl(Span<const CstrView> const& messages)
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            System::ExitFailure();

        for (auto i = size_t{}; i < messages.GetSize(); i++)
        {
            auto const& message = messages[i];
            auto written = uint32_t{};

            if (!WinApi::WriteFile(GPrintHandle, message, StrLen(message), &written, nullptr))
                System::ExitFailure();
        }
    }


    void Console::Error_Impl(Span<const CstrView> const& messages)
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            System::ExitFailure();

        for (auto i = size_t{}; i < messages.GetSize(); i++)
        {
            auto const& message = messages[i];
            auto written = uint32_t{};

            if (!WinApi::WriteFile(GErrorHandle, message, StrLen(message), &written, nullptr))
                System::ExitFailure();
        }
    }


    SpinLock Console::GLock;
    bool Console::GInitialized = false;
    WinApi::HANDLE Console::GPrintHandle = nullptr;
    WinApi::HANDLE Console::GErrorHandle = nullptr;
}


// ------------------------------------
// "ensures.hpp"


#define CheckNoEntry_Stringify(x) #x
#define CheckNoEntry_ToString(x) CheckNoEntry_Stringify(x)
#define CheckNoEntry_GetCodeLine() CheckNoEntry_ToString(__LINE__)


#define CheckNoEntry() \
{ \
    Aiva::Console::ErrorLine(__FILE__, "(", CheckNoEntry_GetCodeLine(), "): ", __func__, ": NO_ENTRY_POINT."); \
    Aiva::System::ExitFailure(); \
} \


// ------------------------------------
// "delete.hpp


void operator delete(void *const ptr, size_t const size);


// ------------------------------------
// "delete.inl


void operator delete(void *const, size_t const)
{
    CheckNoEntry();
}


// ------------------------------------
// "i_allocator.hpp"


namespace Aiva
{
    class IAllocator
    {
    public:
        virtual Span<byte_t> Alloc(size_t const size) const;
        virtual nullptr_t Free(Span<byte_t> const& span) const;

        // ------------------

        template <typename TType>
        Span<TType> AllocArray(size_t const size) const;

        template <typename TType>
        nullptr_t FreeArray(Span<TType> const& span) const;

        // ------------------

        template <typename TType, typename... TArgs>
        TType& Create(TArgs&&... args) const;

        template <typename TType, typename... TArgs>
        Span<TType> CreateArray(size_t const size, TArgs&&... args) const;

        template <typename TType>
        nullptr_t Delete(TType& data) const;

        template <typename TType>
        nullptr_t DeleteArray(Span<TType> const& data) const;

    protected:
        virtual ~IAllocator() = default;
    };
}


// ------------------------------------
// "i_allocator.inl"


namespace Aiva
{
    Span<byte_t> IAllocator::Alloc(size_t const) const
    {
        CheckNoEntry();
    }


    nullptr_t IAllocator::Free(Span<byte_t> const&) const
    {
        CheckNoEntry();
    }


    template <typename TType>
    Span<TType> IAllocator::AllocArray(size_t const size) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType) * size);
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObjects = CastSpan<TType>(spanOfBytes);
        if (!spanOfObjects)
            CheckNoEntry();

        return spanOfObjects;
    }


    template <typename TType>
    nullptr_t IAllocator::FreeArray(Span<TType> const& span) const
    {
        auto const spanOfObjects = span;
        if (!spanOfObjects)
            CheckNoEntry();

        auto const spanOfBytes = CastSpan<byte_t>(spanOfObjects);
        if (!spanOfBytes)
            CheckNoEntry();

        return Free(spanOfBytes);
    }


    template <typename TType, typename... TArgs>
    TType& IAllocator::Create(TArgs&&... args) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType));
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObject = CastSpan<TType>(spanOfBytes);
        if (!spanOfObject)
            CheckNoEntry();

        auto& object = spanOfObject.GetDataRef();
        new (&object) TType{ Templates::Forward<TArgs>(args)... };

        return object;
    }


    template <typename TType, typename... TArgs>
    Span<TType> IAllocator::CreateArray(size_t const size, TArgs&&... args) const
    {
        auto const spanOfBytes = Alloc(sizeof(TType) * size);
        if (!spanOfBytes)
            CheckNoEntry();

        auto const spanOfObjects = CastSpan<TType>(spanOfBytes);
        if (!spanOfObjects)
            CheckNoEntry();

        for (auto i = size_t{}; i < spanOfObjects.GetSize(); i++)
        {
            auto& object = spanOfObjects[i];
            new (&object) TType{ Templates::Forward<TArgs>(args)... };
        }

        return spanOfObjects;
    }


    template <typename TType>
    nullptr_t IAllocator::Delete(TType& data) const
    {
        auto const spanOfObject = Span{ data };
        if (!spanOfObject)
            CheckNoEntry();

        auto const spanOfBytes = CastSpan<byte_t>(spanOfObject);
        if (!spanOfBytes)
            CheckNoEntry();

        data.~TType();
        return Free(spanOfBytes);
    }


    template <typename TType>
    nullptr_t IAllocator::DeleteArray(Span<TType> const& data) const
    {
        for (auto i = data.GetSize(); i > size_t{}; i--)
        {
            auto& object = data[i - 1];
            object.~TType();
        }

        auto const spanOfBytes = CastSpan<byte_t>(data);
        if (!spanOfBytes)
            CheckNoEntry();

        return Free(spanOfBytes);
    }
}


// ------------------------------------
// "memory.hpp"


namespace Aiva
{
    class Memory final
    {
    public:
        static void InitSystem();
        static void ShutSystem();

        static IAllocator const& GetHeapAlloc();

    private:
        class HeapAlloc final : public NonCopyable, public IAllocator
        {
        public:
            HeapAlloc();
            ~HeapAlloc() override;

            Span<byte_t> Alloc(size_t const size) const override;
            nullptr_t Free(Span<byte_t> const& span) const override;

        private:
            WinApi::HANDLE m_heap;
        };

        Memory() = delete;

        static SpinLock GLock;
        static bool GInitialized;
        static ManageObject<HeapAlloc> GHeapAlloc;
    };
}


// ------------------------------------
// "memory.inl"


namespace Aiva
{
    Memory::HeapAlloc::HeapAlloc()
    {
        m_heap = WinApi::HeapCreate(WinApi::DWORD{}, WinApi::SIZE_T{}, WinApi::SIZE_T{});
        if (!m_heap)
            CheckNoEntry();
    }


    Memory::HeapAlloc::~HeapAlloc()
    {
        if (!WinApi::HeapDestroy(m_heap))
            CheckNoEntry();
        m_heap = nullptr;
    }


    Span<byte_t> Memory::HeapAlloc::Alloc(size_t const size) const
    {
        if (size <= size_t{})
            CheckNoEntry();

        auto const data = WinApi::HeapAlloc(m_heap, WinApi::DWORD{}, size);
        if (!data)
            CheckNoEntry();

        return Span<byte_t>{ size, (byte_t*)data };
    }


    nullptr_t Memory::HeapAlloc::Free(Span<byte_t> const& span) const
    {
        if (!span)
            CheckNoEntry();

        if (!WinApi::HeapFree(m_heap, WinApi::DWORD{}, span.GetDataPtr()))
            CheckNoEntry();

        return nullptr;
    }


    void Memory::InitSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (GInitialized)
            CheckNoEntry();

        GHeapAlloc.Construct();
        GInitialized = true;
    }


    void Memory::ShutSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            CheckNoEntry();

        GHeapAlloc.Destruct();
        GInitialized = false;
    }


    IAllocator const& Memory::GetHeapAlloc()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            CheckNoEntry();

        return GHeapAlloc.GetObjectRef();
    }


    SpinLock Memory::GLock;
    bool Memory::GInitialized = false;
    ManageObject<Memory::HeapAlloc> Memory::GHeapAlloc;
}


// ------------------------------------
// "linked_list.hpp"


namespace Aiva
{
    template <typename TType>
    class LinkedList final : public NonCopyable
    {
    public:
        LinkedList() = default;
        ~LinkedList();

        bool IsEmpty() const;
        void PushBack(TType const& data);
        void PushBack(TType&& data);
        TType PopFront();

    public:
        template <typename TCond>
        TType PopFirst(TCond const& condition);

    private:
        struct Node final
        {
            TType m_data = TType{};
            Node* m_next = nullptr;
        };

        Node* m_head = nullptr;
        Node* m_tail = nullptr;
    };
}


// ------------------------------------
// "linked_list.inl"


namespace Aiva
{
    template <typename TType>
    LinkedList<TType>::~LinkedList()
    {
        auto node = m_head;

        while (node)
        {
            auto const next = node->m_next;
            Memory::GetHeapAlloc().Delete(*node);

            node = next;
        }
    }


    template <typename TType>
    bool LinkedList<TType>::IsEmpty() const
    {
        return !m_head;
    }


    template <typename TType>
    void LinkedList<TType>::PushBack(TType const& data)
    {
        auto const node = &Memory::GetHeapAlloc().Create<Node>(data);

        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            m_tail = node;
        }
    }


    template <typename TType>
    void LinkedList<TType>::PushBack(TType&& data)
    {
        auto const node = &Memory::GetHeapAlloc().Create<Node>(Templates::Move(data));

        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            m_tail->m_next = node;
            m_tail = node;
        }
    }


    template <typename TType>
    TType LinkedList<TType>::PopFront()
    {
        if (!m_head)
            return TType{};

        auto const data = Templates::Move(m_head->m_data);

        if (m_head == m_tail)
        {
            Memory::GetHeapAlloc().Delete(*m_head);

            m_head = nullptr;
            m_tail = nullptr;
        }
        else
        {
            auto const head = m_head->m_next;
            Memory::GetHeapAlloc().Delete(*m_head);

            m_head = head;
        }

        return data;
    }


    template <typename TType>
    template <typename TCond>
    TType LinkedList<TType>::PopFirst(TCond const& condition)
    {
        auto prevNode = (Node*)nullptr;
        auto currNode = m_head;

        while (currNode && !condition((TType const&)currNode->m_data))
        {
            prevNode = currNode;
            currNode = currNode->m_next;
        }

        if (!currNode)
            return TType{};

        auto const data = Templates::Move(currNode->m_data);

        if (prevNode)
            prevNode->m_next = currNode->m_next;

        if (currNode == m_head)
            m_head = currNode->m_next;

        if (currNode == m_tail)
            m_tail = prevNode;

        Memory::GetHeapAlloc().Delete(*currNode);
        return data;
    }
}


// ------------------------------------
// "coroutines.hpp"


namespace Aiva
{
    class Coroutines final
    {
    public:
        class ICoroutineControl
        {
        public:
            virtual void Yield() const { CheckNoEntry(); }
            virtual void YieldOnTheCurrentWorker() const { CheckNoEntry(); }
            virtual void Close() const { CheckNoEntry(); }

            virtual ~ICoroutineControl() = default;
        };


        static void InitSystem();
        static void ShutSystem();

        template <typename TAction>
        static void Spawn(TAction&& action);

    private:
        static auto constexpr kAnyWorkerMask = (uintptr_t)(-1);


        class ACoroutine : public NonCopyable
        {
        public:
            uintptr_t GetWorkerMask() const;
            void SetWorkerMask(uintptr_t const workerMask);

            WinApi::LPVOID GetParentFiberHandle();
            void SetParentFiberHandle(WinApi::LPVOID const fiberHandle);

            WinApi::LPVOID GetOrCreateFiberHandle();

            virtual ~ACoroutine();

        protected:
            virtual void Execute(ICoroutineControl const&) const { CheckNoEntry(); }

        private:
            __attribute__((stdcall)) static void NativeAction(WinApi::LPVOID lpFiberParameter);

            uintptr_t m_workerMask = kAnyWorkerMask;
            WinApi::LPVOID m_parentFiberHandle = nullptr;
            WinApi::LPVOID m_fiberHandle = nullptr;
        };


        class CoroutineContol final : public NonCopyable, public ICoroutineControl
        {
        public:
            CoroutineContol(ACoroutine& coroutine);

            void Yield() const override;
            void YieldOnTheCurrentWorker() const override;
            void Close() const override;

        private:
            ACoroutine& m_coroutine;
        };


        class CoroutineQueue final : public NonCopyable
        {
        public:
            void Enqueue(ACoroutine& coroutine);
            ACoroutine* Dequeue(uintptr_t const workerMask);

        private:
            SpinLock m_lock;
            LinkedList<ACoroutine*> m_data;
        };


        class WorkerThread final : public NonCopyable
        {
        public:
            WorkerThread(uintptr_t const workerIndex, CoroutineQueue& coroutineQueue);
            ~WorkerThread();

        private:
            __attribute__((stdcall)) static WinApi::DWORD NativeAction(WinApi::LPVOID lpParameter);

            uintptr_t m_workerIndex;
            CoroutineQueue& m_coroutineQueue;
            WinApi::HANDLE m_threadHandle;
            WinApi::LPVOID m_fiberHandle;
            volatile uintptr_t m_stopFlag;
        };


        Coroutines() = delete;

        static SpinLock GLock;
        static bool GInitialized;
        static ManageObject<CoroutineQueue> GCoroutineQueue;
        static Span<WorkerThread> GWorkerThreads;
    };
}


// ------------------------------------
// "coroutines.inl"


namespace Aiva
{
    void Coroutines::InitSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (GInitialized)
            CheckNoEntry();

        GCoroutineQueue.Construct();

        GWorkerThreads = Memory::GetHeapAlloc().AllocArray<WorkerThread>(System::GetNumberOfCores());
        for (auto i = size_t{}; i < GWorkerThreads.GetSize(); i++)
            new (&GWorkerThreads[i]) WorkerThread(i, *GCoroutineQueue);

        GInitialized = true;
    }


    void Coroutines::ShutSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            CheckNoEntry();

        for (auto i = GWorkerThreads.GetSize(); i > 0u; i--)
            GWorkerThreads[i - 1u].~WorkerThread();
        GWorkerThreads = Span<WorkerThread>{};

        GCoroutineQueue.Destruct();

        GInitialized = false;
    }


    template <typename TAction>
    void Coroutines::Spawn(TAction&& action)
    {
        class Coroutine final : public ACoroutine
        {
        public:
            Coroutine(TAction&& action) : m_action{ Templates::Move(action) } {}
        protected:
            void Execute(ICoroutineControl const& coroutineControl) const override { m_action(coroutineControl); }
        private:
            TAction m_action;
        };


        auto& coroutine = Memory::GetHeapAlloc().Create<Coroutine>(Templates::Move(action));

        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            CheckNoEntry();

        GCoroutineQueue->Enqueue(coroutine);
    }


    uintptr_t Coroutines::ACoroutine::GetWorkerMask() const
    {
        return m_workerMask;
    }


    void Coroutines::ACoroutine::SetWorkerMask(uintptr_t const workerMask)
    {
        m_workerMask = workerMask;
    }


    WinApi::LPVOID Coroutines::ACoroutine::GetParentFiberHandle()
    {
        return m_parentFiberHandle;
    }


    void Coroutines::ACoroutine::SetParentFiberHandle(WinApi::LPVOID const fiberHandle)
    {
        m_parentFiberHandle = fiberHandle;
    }


    WinApi::LPVOID Coroutines::ACoroutine::GetOrCreateFiberHandle()
    {
        if (m_fiberHandle)
            return m_fiberHandle;

        if (!WinApi::IsThreadAFiber())
            CheckNoEntry();

        m_fiberHandle = WinApi::CreateFiber(16384, NativeAction, this);
        if (!m_fiberHandle)
            CheckNoEntry();

        return m_fiberHandle;
    }


    Coroutines::ACoroutine::~ACoroutine()
    {
        if (m_fiberHandle)
            WinApi::DeleteFiber(m_fiberHandle);
    }


    __attribute__((stdcall)) void Coroutines::ACoroutine::NativeAction(WinApi::LPVOID lpFiberParameter)
    {
        auto const coroutine = (ACoroutine*)lpFiberParameter;
        if (!coroutine)
            CheckNoEntry();

        CoroutineContol const coroutineContol{ *coroutine };

        coroutine->Execute(coroutineContol);
        coroutineContol.Close();
    }


    Coroutines::CoroutineContol::CoroutineContol(ACoroutine& coroutine)
        : m_coroutine{ coroutine }
    {
        //
    }


    void Coroutines::CoroutineContol::Yield() const
    {
        m_coroutine.SetWorkerMask(kAnyWorkerMask);

        auto const parentFiberHandle = m_coroutine.GetParentFiberHandle();
        if (!parentFiberHandle)
            CheckNoEntry();

        WinApi::SwitchToFiber(parentFiberHandle);
    }


    void Coroutines::CoroutineContol::YieldOnTheCurrentWorker() const
    {
        if (!m_coroutine.GetWorkerMask())
            CheckNoEntry();

        auto const parentFiberHandle = m_coroutine.GetParentFiberHandle();
        if (!parentFiberHandle)
            CheckNoEntry();

        WinApi::SwitchToFiber(parentFiberHandle);
    }


    void Coroutines::CoroutineContol::Close() const
    {
        m_coroutine.SetWorkerMask(0u);

        auto const parentFiberHandle = m_coroutine.GetParentFiberHandle();
        if (!parentFiberHandle)
            CheckNoEntry();

        WinApi::SwitchToFiber(parentFiberHandle);
    }


    void Coroutines::CoroutineQueue::Enqueue(ACoroutine& coroutine)
    {
        SpinLockScope_t const lockScope{ m_lock };
        m_data.PushBack(&coroutine);
    }


    Coroutines::ACoroutine* Coroutines::CoroutineQueue::Dequeue(uintptr_t const workerMask)
    {
        SpinLockScope_t const lockScope{ m_lock };
        return m_data.PopFirst([&](auto const c) { return c && (c->GetWorkerMask() & workerMask) != uintptr_t{}; });
    }


    Coroutines::WorkerThread::WorkerThread(uintptr_t const workerIndex, CoroutineQueue& coroutineQueue)
        : m_workerIndex{ workerIndex }, m_coroutineQueue{ coroutineQueue }, m_stopFlag{ false }
    {
        m_threadHandle = WinApi::CreateThread(nullptr, 16384, NativeAction, this, 0u, nullptr);
        if (!m_threadHandle)
            CheckNoEntry();
    }


    Coroutines::WorkerThread::~WorkerThread()
    {
        Intrin::AtomicExchange(&m_stopFlag, true);

        if (WinApi::WaitForSingleObject(m_threadHandle, WinApi::INFINITE) == WinApi::WAIT_FAILED)
            CheckNoEntry();

        if (!WinApi::CloseHandle(m_threadHandle))
            CheckNoEntry();
    }


    __attribute__((stdcall)) WinApi::DWORD Coroutines::WorkerThread::NativeAction(WinApi::LPVOID lpParameter)
    {
        auto const self = (WorkerThread*)lpParameter;
        if (!self)
            CheckNoEntry();

        self->m_fiberHandle = WinApi::ConvertThreadToFiber(nullptr);
        if (!self->m_fiberHandle)
            CheckNoEntry();

        while (Intrin::AtomicCompareExchange(&self->m_stopFlag, false, false) == false)
        {
            auto const coroutine = self->m_coroutineQueue.Dequeue(1u << self->m_workerIndex);
            if (!coroutine)
            {
                Intrin::YieldProcessor();
                continue;
            }

            auto const coroutineFiberHandle = coroutine->GetOrCreateFiberHandle();
            if (!coroutineFiberHandle)
                CheckNoEntry();

            coroutine->SetWorkerMask(1u << self->m_workerIndex);
            coroutine->SetParentFiberHandle(self->m_fiberHandle);
            WinApi::SwitchToFiber(coroutineFiberHandle);
            coroutine->SetParentFiberHandle(nullptr);

            if (!coroutine->GetWorkerMask())
            {
                Memory::GetHeapAlloc().Delete(*coroutine);
                continue;
            }

            self->m_coroutineQueue.Enqueue(*coroutine);
        }

        if (!WinApi::ConvertFiberToThread())
            CheckNoEntry();
        self->m_fiberHandle = nullptr;

        return 0u;
    }


    SpinLock Coroutines::GLock;
    bool Coroutines::GInitialized = false;
    ManageObject<Coroutines::CoroutineQueue> Coroutines::GCoroutineQueue;
    Span<Coroutines::WorkerThread> Coroutines::GWorkerThreads;
}


// --------------------------------------------------------
// "windows.hpp"


namespace Aiva
{
    class Windows final
    {
    public:
        static void InitSystem();
        static void ShutSystem();

    private:
        static auto constexpr kWindowClassName = L"Aiva::Window";
        static auto constexpr kWindowName = L"aiva";


        class WindowClass final : public NonCopyable
        {
        public:
            WindowClass();
            ~WindowClass();
        };


        class Window final : public NonCopyable
        {
        public:
            Window();
            ~Window();

        private:
            void WindowCoroutine(Coroutines::ICoroutineControl const& control);

            volatile uintptr_t m_stopFlag;
            WinApi::HWND m_windowHandle;
        };


        Windows() = delete;

        static SpinLock GLock;
        static bool GInitialized;
        static ManageObject<WindowClass> GWindowClass;
        static ManageObject<Window> GWindow;
    };
}


// --------------------------------------------------------
// "windows.cpp"


namespace Aiva
{
    Windows::WindowClass::WindowClass()
    {
        auto windowClass = WinApi::WNDCLASSEXW{};
        windowClass.cbSize = sizeof(WinApi::WNDCLASSEXW);
        windowClass.lpfnWndProc = WinApi::DefWindowProcW;
        windowClass.hInstance = WinApi::GetModuleHandleW(nullptr);
        windowClass.lpszClassName = kWindowClassName;

        if (WinApi::RegisterClassExW(&windowClass) == 0)
            CheckNoEntry();
    }


    Windows::WindowClass::~WindowClass()
    {
        auto const lpszClassName = kWindowClassName;
        auto const hInstance = WinApi::GetModuleHandleW(nullptr);

        if (!WinApi::UnregisterClassW(lpszClassName, hInstance))
            CheckNoEntry();
    }


    Windows::Window::Window()
    {
        Intrin::AtomicExchange(&m_stopFlag, false);

        Coroutines::Spawn([this](auto const& control)
            { WindowCoroutine(control); });
    }


    Windows::Window::~Window()
    {
        Intrin::AtomicExchange(&m_stopFlag, true);
    }


    void Windows::Window::WindowCoroutine(Coroutines::ICoroutineControl const& control)
    {
        m_windowHandle = WinApi::CreateWindowExW
        (
            /*dwExStyle*/ 0u,
            /*lpClassName*/ kWindowClassName,
            /*lpWindowName*/ kWindowName,
            /*dwStyle*/ WinApi::WS_OVERLAPPEDWINDOW,
            /*X*/ 0,
            /*Y*/ 0,
            /*nWidth*/ 1280,
            /*nHeight*/ 720,
            /*hWndParent*/ nullptr,
            /*hMenu*/ nullptr,
            /*hInstance*/ WinApi::GetModuleHandleW(nullptr),
            /*lpParam*/ nullptr
        );
        if (!m_windowHandle)
            CheckNoEntry();

        auto windowRect = WinApi::RECT{};
        if (!WinApi::GetWindowRect(m_windowHandle, &windowRect))
            CheckNoEntry();

        auto const monitor = WinApi::MonitorFromWindow(m_windowHandle, WinApi::MONITOR_DEFAULTTONEAREST);
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
            /* hWnd */ m_windowHandle,
            /* hWndInsertAfter */ WinApi::HWND_TOP,
            /* X */ posX,
            /* Y */ posY,
            /* cx */ 0,
            /* cy */ 0,
            /* uFlags */ WinApi::SWP_NOSIZE | WinApi::SWP_NOZORDER | WinApi::SWP_SHOWWINDOW
        ))
            CheckNoEntry();

        while (Intrin::AtomicCompareExchange(&m_stopFlag, false, false) == false)
        {
            auto msg = WinApi::MSG{};

            while (WinApi::PeekMessageW(&msg, m_windowHandle, 0u, 0u, WinApi::PM_REMOVE))
            {
                WinApi::TranslateMessage(&msg);
                WinApi::DispatchMessageW(&msg);
            }

            control.YieldOnTheCurrentWorker();
        }

        if (!WinApi::DestroyWindow(m_windowHandle))
            CheckNoEntry();
        m_windowHandle = nullptr;
    }


    void Windows::InitSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (GInitialized)
            return;

        GWindowClass.Construct();
        GWindow.Construct();

        GInitialized = true;
    }


    void Windows::ShutSystem()
    {
        SpinLockScope_t const lockScope{ GLock };
        if (!GInitialized)
            return;

        GWindow.Destruct();
        GWindowClass.Destruct();

        GInitialized = false;
    }


    SpinLock Windows::GLock;
    bool Windows::GInitialized = false;
    ManageObject<Windows::WindowClass> Windows::GWindowClass;
    ManageObject<Windows::Window> Windows::GWindow;
}
