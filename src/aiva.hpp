#pragma once
// です。


// ========================================================
// Module: Basic Templates
// ========================================================


namespace Aiva::Templates
{
    template <typename...>
    using Void_t = void;
}


// ========================================================
// Module: Number Templates
// ========================================================


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


// --------------------------------------------------------


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


// ========================================================
// Module: Primitive Types
// ========================================================


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
}
