#pragma once
// です。


// ------------------------------------
// "basic_templates.hpp"


namespace Aiva::Templates
{
    template <typename...>
    using Void_t = void;


    template <typename TType>
    constexpr TType&& Forward(TType& value);


    template <typename TType>
    constexpr TType&& Forward(TType&& value);

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

    using UINT = uint32_t;

    // Process Management

    [[noreturn]] __attribute__((dllimport, stdcall)) void ExitProcess(UINT const uExitCode);
}}


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
// "process.hpp"


namespace Aiva::Process
{
    [[noreturn]] void ExitSuccess();
    [[noreturn]] void ExitFailure();
}


// ------------------------------------
// "process.inl"


namespace Aiva::Process
{
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
