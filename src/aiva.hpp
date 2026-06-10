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
