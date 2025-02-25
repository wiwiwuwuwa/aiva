#pragma once
#include "EArithmeticType.hpp"
#include "TIntegralConstant.hpp"

namespace Aiva::MinLib
{
    template <typename T>
    struct TGetArithmeticType;

    template <>
    struct TGetArithmeticType<char> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<signed char> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<unsigned char> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<short int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<unsigned short int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<unsigned int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<long int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<unsigned long int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<long long int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<unsigned long long int> : public TIntegralConstant<EArithmeticType, EArithmeticType::Integral> {};

    template <>
    struct TGetArithmeticType<float> : public TIntegralConstant<EArithmeticType, EArithmeticType::Floating> {};

    template <>
    struct TGetArithmeticType<double> : public TIntegralConstant<EArithmeticType, EArithmeticType::Floating> {};

    template <>
    struct TGetArithmeticType<long double> : public TIntegralConstant<EArithmeticType, EArithmeticType::Floating> {};
}
// namespace Aiva::MinLib
