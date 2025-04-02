#pragma once

namespace aiva
{
    enum class NumberType
    {
        Int,
        Flt,
    };

    enum class NumberSign
    {
        None,
        Sign,
    };

    enum class NumberSize
    {
        Byte1,
        Byte2,
        Byte4,
        Byte8,
    };

    template <typename TType>
    constexpr bool IsInteger()
    {
        return TType(3) / TType(2) == TType(1);
    }

    template <typename TType>
    constexpr bool IsFloating()
    {
        return TType(3) / TType(2) != TType(1);
    }

    template <typename TType>
    constexpr bool IsUnsigned()
    {
        return TType(-1) >= TType(0);
    }

    template <typename TType>
    constexpr bool IsSigned()
    {
        return TType(-1) < TType(0);
    }

    template <typename TType>
    constexpr NumberType GetNumberType()
    {
        if constexpr (IsInteger<TType>())
            { return NumberType::Int; }

        else if constexpr (IsFloating<TType>())
            { return NumberType::Flt; }

        else
            { static_assert(false); }
    }

    template <typename TType>
    constexpr NumberSign GetNumberSign()
    {
        if constexpr (IsSigned<TType>())
            { return NumberSign::Sign; }

        else if constexpr (IsUnsigned<TType>())
            { return NumberSign::None; }

        else
            { static_assert(false); }
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
            { static_assert(false); }
    }

    template <NumberType TType, NumberSign TSign, NumberSize TSize>
    constexpr auto CreateNumber()
    {
        if constexpr (TType == GetNumberType<char>() && TSign == GetNumberSign<char>() && TSize == GetNumberSize<char>())
            { char const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<signed char>() && TSign == GetNumberSign<signed char>() && TSize == GetNumberSize<signed char>())
            { signed char const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<unsigned char>() && TSign == GetNumberSign<unsigned char>() && TSize == GetNumberSize<unsigned char>())
            { unsigned char const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<short int>() && TSign == GetNumberSign<short int>() && TSize == GetNumberSize<short int>())
            { short int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<unsigned short int>() && TSign == GetNumberSign<unsigned short int>() && TSize == GetNumberSize<unsigned short int>())
            { unsigned short int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<int>() && TSign == GetNumberSign<int>() && TSize == GetNumberSize<int>())
            { int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<unsigned int>() && TSign == GetNumberSign<unsigned int>() && TSize == GetNumberSize<unsigned int>())
            { unsigned int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<long int>() && TSign == GetNumberSign<long int>() && TSize == GetNumberSize<long int>())
            { long int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<unsigned long int>() && TSign == GetNumberSign<unsigned long int>() && TSize == GetNumberSize<unsigned long int>())
            { unsigned long int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<long long int>() && TSign == GetNumberSign<long long int>() && TSize == GetNumberSize<long long int>())
            { long long int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<unsigned long long int>() && TSign == GetNumberSign<unsigned long long int>() && TSize == GetNumberSize<unsigned long long int>())
            { unsigned long long int const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<float>() && TSign == GetNumberSign<float>() && TSize == GetNumberSize<float>())
            { float const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<double>() && TSign == GetNumberSign<double>() && TSize == GetNumberSize<double>())
            { double const zero{}; return zero; }

        else if constexpr (TType == GetNumberType<long double>() && TSign == GetNumberSign<long double>() && TSize == GetNumberSize<long double>())
            { long double const zero{}; return zero; }

        else
            { static_assert(false); }
    }

    using int8_t = decltype(CreateNumber<NumberType::Int, NumberSign::Sign, NumberSize::Byte1>());
    using int16_t = decltype(CreateNumber<NumberType::Int, NumberSign::Sign, NumberSize::Byte2>());
    using int32_t = decltype(CreateNumber<NumberType::Int, NumberSign::Sign, NumberSize::Byte4>());
    using int64_t = decltype(CreateNumber<NumberType::Int, NumberSign::Sign, NumberSize::Byte8>());
    using uint8_t = decltype(CreateNumber<NumberType::Int, NumberSign::None, NumberSize::Byte1>());
    using uint16_t = decltype(CreateNumber<NumberType::Int, NumberSign::None, NumberSize::Byte2>());
    using uint32_t = decltype(CreateNumber<NumberType::Int, NumberSign::None, NumberSize::Byte4>());
    using uint64_t = decltype(CreateNumber<NumberType::Int, NumberSign::None, NumberSize::Byte8>());
    using float32_t = decltype(CreateNumber<NumberType::Flt, NumberSign::None, NumberSize::Byte4>());
    using float64_t = decltype(CreateNumber<NumberType::Flt, NumberSign::None, NumberSize::Byte8>());
    using intptr_t = decltype(CreateNumber<NumberType::Int, NumberSign::Sign, GetNumberSize<void*>()>());
    using uintptr_t = decltype(CreateNumber<NumberType::Int, NumberSign::None, GetNumberSize<void*>()>());

    enum class byte_t : uint8_t {};
}
// namespace aiva
