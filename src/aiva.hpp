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
    constexpr auto GetNumberType() -> NumberType;


    template <typename TType>
    constexpr auto GetNumberSign() -> NumberSign;


    template <typename TType>
    constexpr auto GetNumberSize() -> NumberSize;


    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber();


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
    constexpr auto GetNumberType() -> NumberType
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
    constexpr auto GetNumberSign() -> NumberSign
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
    constexpr auto GetNumberSize() -> NumberSize
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
// "intrin.hpp"


namespace Aiva::Intrin
{
    template <typename TType>
    inline auto AtomicCompareExchange(volatile TType *const dst, TType const cmp, TType const exg) -> TType;


    template <typename TType>
    inline auto AtomicExchange(volatile TType *const dst, TType const exg) -> TType;


    inline void YieldProcessor();
}


// ------------------------------------
// "intrin.inl"


namespace Aiva::Intrin
{
    template <typename TType>
    inline auto AtomicCompareExchange(volatile TType *const dst, TType const cmp, TType const exg) -> TType
    {
        static_assert(sizeof(TType) == 1 || sizeof(TType) == 2 || sizeof(TType) == 4 || sizeof(TType) == 8, "TType must be 1, 2, 4, or 8 bytes");
        auto prev = cmp;
        __atomic_compare_exchange_n(dst, &prev, exg, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);

        return prev;
    }


    template <typename TType>
    inline auto AtomicExchange(volatile TType *const dst, TType const exg) -> TType
    {
        static_assert(sizeof(TType) == 1 || sizeof(TType) == 2 || sizeof(TType) == 4 || sizeof(TType) == 8, "TType must be 1, 2, 4, or 8 bytes");
        return __atomic_exchange_n(dst, exg, __ATOMIC_SEQ_CST);
    }


    inline void YieldProcessor()
    {
        __builtin_ia32_pause();
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
    class SpinLock : public NonCopyable
    {
    public:
        SpinLock() = default;
        ~SpinLock() = default;

        void Lock();
        void Unlock();

    private:
        volatile uintptr_t m_locked = 0;
    };
}


// ------------------------------------
// "spin_lock.inl"


namespace Aiva
{
    void SpinLock::Lock()
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange<uintptr_t>(&m_locked, 0, 1) == 0)
                break;

            Intrin::YieldProcessor();
        }
    }


    void SpinLock::Unlock()
    {
        while (true)
        {
            if (Intrin::AtomicCompareExchange<uintptr_t>(&m_locked, 1, 0) == 1)
                break;

            Intrin::YieldProcessor();
        }
    }
}
