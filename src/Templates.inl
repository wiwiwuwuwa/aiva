#pragma once
#include "Templates.hpp"


namespace Aiva::Templates
{
    // ========================================================================
    // Basic
    // ========================================================================

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

    // ========================================================================
    // Numbers
    // ========================================================================

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
            { return NumberType::MAX; }
    }


    template <typename TType>
    constexpr NumberSign GetNumberSign()
    {
        if constexpr (IsSigned<TType>())
            { return NumberSign::Sign; }

        else if constexpr (IsUnsigned<TType>())
            { return NumberSign::None; }

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
        if constexpr (TType == NumberType::MAX || TSign == NumberSign::MAX || TSize == NumberSize::MAX)
            { return nullptr; }

        else if constexpr (TType == GetNumberType<char>() && TSign == GetNumberSign<char>() && TSize == GetNumberSize<char>())
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
            { return nullptr; }
    }
}
// namespace Aiva::Templates
