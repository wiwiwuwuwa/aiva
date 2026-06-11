#pragma once
// です。


// ------------------------------------
// "basic_templates.hpp"


namespace Aiva::Templates
{
    template <typename...>
    using Void_t = void;
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
    inline constexpr auto IsNumber_v = false;


    template <typename TType>
    inline constexpr auto IsNumber_v<TType, Void_t<decltype(TType{ 3 } / TType{ 2 })>> = true;


    template <typename TType>
    constexpr auto GetNumberType() noexcept -> NumberType;


    template <typename TType>
    constexpr auto GetNumberSign() noexcept -> NumberSign;


    template <typename TType>
    constexpr auto GetNumberSize() noexcept -> NumberSize;


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber() noexcept;


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    using Number_t = decltype(CreateNumber<TType, TSign, TSize>());


    inline constexpr auto POINTER_SIZE
        = sizeof(void*) == 1 ? NumberSize::Byte1
        : sizeof(void*) == 2 ? NumberSize::Byte2
        : sizeof(void*) == 4 ? NumberSize::Byte4
        : sizeof(void*) == 8 ? NumberSize::Byte8
        : NumberSize::MAX;
}


// ------------------------------------
// "number_templates.inl"


namespace Aiva::Templates
{
    template <typename TType>
    constexpr auto GetNumberType() noexcept -> NumberType
    {
        if constexpr (IsNumber_v<TType>)
        {
            if constexpr ((TType)(3) / (TType)(2) == (TType)(1))
                { return NumberType::Int; }
            else
                { return NumberType::Flt; }
        }
        else
        {
            return NumberType::MAX;
        }
    }


    template <typename TType>
    constexpr auto GetNumberSign() noexcept -> NumberSign
    {
        if constexpr (IsNumber_v<TType>)
        {
            if constexpr ((TType)(-1) < (TType)(0))
                { return NumberSign::Sign; }
            else
                { return NumberSign::None; }
        }
        else
        {
            return NumberSign::MAX;
        }
    }


    template <typename TType>
    constexpr auto GetNumberSize() noexcept -> NumberSize
    {
        if constexpr (IsNumber_v<TType>)
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
        else
        {
            return NumberSize::MAX;
        }
    }


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber() noexcept
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
    using sint8_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::Sign, Templates::NumberSize::Byte1>;
    using sint16_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::Sign, Templates::NumberSize::Byte2>;
    using sint32_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::Sign, Templates::NumberSize::Byte4>;
    using sint64_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::Sign, Templates::NumberSize::Byte8>;
    using uint8_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::NumberSize::Byte1>;
    using uint16_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::NumberSize::Byte2>;
    using uint32_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::NumberSize::Byte4>;
    using uint64_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::NumberSize::Byte8>;
    using float32_t = Templates::Number_t<Templates::NumberType::Flt, Templates::NumberSign::Sign, Templates::NumberSize::Byte4>;
    using float64_t = Templates::Number_t<Templates::NumberType::Flt, Templates::NumberSign::Sign, Templates::NumberSize::Byte8>;
    using sintptr_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::Sign, Templates::POINTER_SIZE>;
    using uintptr_t = Templates::Number_t<Templates::NumberType::Int, Templates::NumberSign::None, Templates::POINTER_SIZE>;

    enum class byte_t : uint8_t {};
    using size_t = decltype(sizeof(byte_t));
    using nullptr_t = decltype(nullptr);
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
// "cstr_view.hpp"


namespace Aiva
{
    class CstrView final
    {
    public:
        constexpr CstrView() noexcept;
        constexpr CstrView(char const*const data) noexcept;

        constexpr bool IsEmpty() const noexcept;
        constexpr size_t StrLen() const noexcept;
        constexpr char const* GetDataPtr() const noexcept;

        constexpr operator bool() const noexcept;
        constexpr char operator[](size_t const index) const noexcept;
        constexpr operator char const*() const noexcept;

    private:
        char const& m_data;
    };
}


// ------------------------------------
// "cstr_view.inl"


namespace Aiva
{
    constexpr CstrView::CstrView() noexcept : m_data{ *"" }
    {
        //
    }


    constexpr CstrView::CstrView(char const*const data) noexcept : m_data{ data ? *data : *"" }
    {
        //
    }


    constexpr bool CstrView::IsEmpty() const noexcept
    {
        return (&m_data)[0] == '\0';
    }


    constexpr size_t CstrView::StrLen() const noexcept
    {
        auto length = size_t{};

        while ((&m_data)[length] != '\0')
            length++;

        return length;
    }


    constexpr char const* CstrView::GetDataPtr() const noexcept
    {
        return &m_data;
    }


    constexpr CstrView::operator bool() const noexcept
    {
        return (&m_data)[0] != '\0';
    }


    constexpr char CstrView::operator[](size_t const index) const noexcept
    {
        return (&m_data)[index];
    }


    constexpr CstrView::operator char const*() const noexcept
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
        constexpr Span() noexcept;
        constexpr Span(size_t const size, TType& data) noexcept;
        constexpr Span(size_t const size, TType *const data) noexcept;
        constexpr Span(TType& data) noexcept;
        constexpr Span(TType *const data) noexcept;
        constexpr Span(nullptr_t const) noexcept;

        constexpr bool IsEmpty() const noexcept;
        constexpr size_t GetSize() const noexcept;
        constexpr TType& GetDataRef() const noexcept;
        constexpr TType* GetDataPtr() const noexcept;

        constexpr operator bool() const noexcept;
        constexpr TType& operator[](size_t const index) const noexcept;

    private:
        size_t m_size;
        TType* m_data;
    };
}


// ------------------------------------
// "span.inl"


namespace Aiva
{
    template <typename TType>
    constexpr Span<TType>::Span() noexcept :
        m_size{ 0 },
        m_data{ nullptr }
    {
        //
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType& data) noexcept :
        m_size{ size > 0 ? size : 0 },
        m_data{ size > 0 ? &data : nullptr }
    {
        //
    }


    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType *const data) noexcept :
        m_size{ size > 0 && data != nullptr ? size : 0 },
        m_data{ size > 0 && data != nullptr ? data : nullptr }
    {
        //
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType& data) noexcept :
        m_size{ 1 },
        m_data{ &data }
    {
        //
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType *const data) noexcept :
        m_size{ data ? size_t{ 1 } : size_t{ 0 } },
        m_data{ data ? data : nullptr }
    {
        //
    }


    template <typename TType>
    constexpr Span<TType>::Span(nullptr_t const) noexcept :
        m_size{ 0 },
        m_data{ nullptr }
    {
        //
    }


    template <typename TType>
    constexpr bool Span<TType>::IsEmpty() const noexcept
    {
        return m_size == 0;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const noexcept
    {
        return m_size;
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetDataRef() const noexcept
    {
        return *m_data;
    }


    template <typename TType>
    constexpr TType* Span<TType>::GetDataPtr() const noexcept
    {
        return m_data;
    }


    template <typename TType>
    constexpr Span<TType>::operator bool() const noexcept
    {
        return m_size > 0;
    }


    template <typename TType>
    constexpr TType& Span<TType>::operator[](size_t const index) const noexcept
    {
        return m_data[index];
    }
}


// ------------------------------------
// "intrin.hpp"


namespace Aiva::Intrin
{
    template <typename TType>
    inline auto AtomicCompareExchange(TType *const dst, TType const cmp, TType const exg) noexcept -> TType;

    template <typename TType>
    inline auto AtomicExchange(TType *const dst, TType const exg) noexcept -> TType;

    inline void YieldProcessor() noexcept;
}


// ------------------------------------
// "intrin.inl"


namespace Aiva::Intrin
{
    template <typename TType>
    inline auto AtomicCompareExchange(TType *const dst, TType const cmp, TType const exg) noexcept -> TType
    {
        static_assert(sizeof(TType) == 1 || sizeof(TType) == 2 || sizeof(TType) == 4 || sizeof(TType) == 8, "TType must be 1, 2, 4, or 8 bytes");
        auto prev = cmp;
        __atomic_compare_exchange_n(dst, &prev, exg, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

        return prev;
    }


    template <typename TType>
    inline auto AtomicExchange(TType *const dst, TType const exg) noexcept -> TType
    {
        static_assert(sizeof(TType) == 1 || sizeof(TType) == 2 || sizeof(TType) == 4 || sizeof(TType) == 8, "TType must be 1, 2, 4, or 8 bytes");
        return __atomic_exchange_n(dst, exg, __ATOMIC_SEQ_CST);
    }


    inline void YieldProcessor() noexcept
    {
        __builtin_ia32_pause();
    }
}


// ------------------------------------
// "winapi.hpp"


extern "C" { namespace Aiva::WinApi
{
    using DWORD = uint32_t;
    using UINT = uint32_t;
    using BOOL = sint32_t;
    using HANDLE = void*;
    using LPCVOID = void const*;
    using LPDWORD = DWORD*;
    using LONG_PTR = sintptr_t;

    struct OVERLAPPED;
    using LPOVERLAPPED = OVERLAPPED*;

    static auto const INVALID_HANDLE_VALUE = (HANDLE)(LONG_PTR)(-1);
    static auto const STD_OUTPUT_HANDLE = (DWORD)(-11);
    static auto const STD_ERROR_HANDLE = (DWORD)(-12);

    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT uExitCode) noexcept;
    __attribute__((dllimport, stdcall)) HANDLE GetStdHandle(DWORD nStdHandle) noexcept;
    __attribute__((dllimport, stdcall)) BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) noexcept;
}}


// ------------------------------------
// "system.hpp"


namespace Aiva::System
{
    [[noreturn]] void ExitSuccess() noexcept;
    [[noreturn]] void ExitFailure() noexcept;
}


// ------------------------------------
// "system.inl"


namespace Aiva::System
{
    [[noreturn]] void ExitSuccess() noexcept
    {
        WinApi::ExitProcess(0);
    }


    [[noreturn]] void ExitFailure() noexcept
    {
        WinApi::ExitProcess(1);
    }
}


// ------------------------------------
// "lock_scope.hpp"


namespace Aiva
{
    template <typename TType>
    class LockScope : public NonCopyable
    {
    public:
        LockScope(TType& lock) noexcept;
        ~LockScope() noexcept;

    private:
        TType& m_lock;
    };
}


// ------------------------------------
// "lock_scope.inl"


namespace Aiva
{
    template <typename TType>
    LockScope<TType>::LockScope(TType& lock) noexcept : m_lock{ lock }
    {
        m_lock.Lock();
    }


    template <typename TType>
    LockScope<TType>::~LockScope() noexcept
    {
        m_lock.Unlock();
    }
}


// ------------------------------------
// "spin_lock.hpp"


namespace Aiva
{
    class SpinLock : public NonCopyable
    {
    public:
        SpinLock() = default;
        ~SpinLock() = default;

        void Lock() noexcept;
        void Unlock() noexcept;

    private:
        uintptr_t m_locked = 0;
    };


    using SpinLockScope_t = LockScope<SpinLock>;
}


// ------------------------------------
// "spin_lock.inl"


namespace Aiva
{
    void SpinLock::Lock() noexcept
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange<uintptr_t>(&m_locked, 0, 1) == 0)
                break;

            Intrin::YieldProcessor();
        }
    }


    void SpinLock::Unlock() noexcept
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange<uintptr_t>(&m_locked, 1, 0) == 1)
                break;

            Intrin::YieldProcessor();
        }
    }
}


// ------------------------------------
// "console.hpp"


namespace Aiva
{
    class Console final
    {
    public:
        static void InitSystem() noexcept;
        static void ShutSystem() noexcept;

        template <typename... TArgs>
        static void Print(TArgs const&... args) noexcept;

        template <typename... TArgs>
        static void PrintLine(TArgs const&... args) noexcept;

        template <typename... TArgs>
        static void Error(TArgs const&... args) noexcept;

        template <typename... TArgs>
        static void ErrorLine(TArgs const&... args) noexcept;

    private:
        Console() = delete;

        static void Print_Impl(Span<const CstrView> const& messages) noexcept;
        static void Error_Impl(Span<const CstrView> const& messages) noexcept;

        inline static SpinLock GLock;
        inline static bool GInitialized;
        inline static WinApi::HANDLE GPrintHandle;
        inline static WinApi::HANDLE GErrorHandle;
    };
}


// ------------------------------------
// "console.inl"


namespace Aiva
{
    void Console::InitSystem() noexcept
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


    void Console::ShutSystem() noexcept
    {
        SpinLockScope_t const lockScope{ GLock };

        if (!GInitialized)
            System::ExitFailure();

        GInitialized = false;
    }


    template <typename... TArgs>
    void Console::Print(TArgs const&... args) noexcept
    {
        CstrView const messages[]{ CstrView{args}... };
        Print_Impl(messages);
    }


    template <typename... TArgs>
    void Console::PrintLine(TArgs const&... args) noexcept
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Print_Impl(messages);
    }


    template <typename... TArgs>
    void Console::Error(TArgs const&... args) noexcept
    {
        CstrView const messages[]{ CstrView{args}... };
        Error_Impl(messages);
    }


    template <typename... TArgs>
    void Console::ErrorLine(TArgs const&... args) noexcept
    {
        CstrView const messages[]{ CstrView{args}..., CstrView{"\n"} };
        Error_Impl(messages);
    }


    void Console::Print_Impl(Span<const CstrView> const& messages) noexcept
    {
        SpinLockScope_t const lockScope{ GLock };

        if (!GInitialized)
            System::ExitFailure();

        for (auto i = size_t{}; i < messages.GetSize(); i++)
        {
            auto const& message = messages[i];
            auto written = uint32_t{};

            if (!WinApi::WriteFile(GPrintHandle, message.GetDataPtr(), message.StrLen(), &written, nullptr))
                System::ExitFailure();
        }
    }


    void Console::Error_Impl(Span<const CstrView> const& messages) noexcept
    {
        SpinLockScope_t const lockScope{ GLock };

        if (!GInitialized)
            System::ExitFailure();

        for (auto i = size_t{}; i < messages.GetSize(); i++)
        {
            auto const& message = messages[i];
            auto written = uint32_t{};

            if (!WinApi::WriteFile(GErrorHandle, message.GetDataPtr(), message.StrLen(), &written, nullptr))
                System::ExitFailure();
        }
    }
}
