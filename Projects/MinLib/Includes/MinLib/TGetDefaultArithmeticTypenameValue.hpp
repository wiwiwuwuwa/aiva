#pragma once
#include "NullptrType.hpp"
#include "TGetArithmeticSign.hpp"
#include "TGetArithmeticSize.hpp"
#include "TGetArithmeticType.hpp"

namespace Aiva::MinLib
{
    template <EArithmeticSign Sign, EArithmeticSize Size, EArithmeticType Type>
    constexpr auto TGetDefaultArithmeticTypenameValue()
    {
        if constexpr (Sign == TGetArithmeticSign<char>::Value && Size == TGetArithmeticSize<char>::Value && Type == TGetArithmeticType<char>::Value)
        {
            return (char){};
        }
        else if constexpr (Sign == TGetArithmeticSign<signed char>::Value && Size == TGetArithmeticSize<signed char>::Value && Type == TGetArithmeticType<signed char>::Value)
        {
            return (signed char){};
        }
        else if constexpr (Sign == TGetArithmeticSign<unsigned char>::Value && Size == TGetArithmeticSize<unsigned char>::Value && Type == TGetArithmeticType<unsigned char>::Value)
        {
            return (unsigned char){};
        }
        else if constexpr (Sign == TGetArithmeticSign<short int>::Value && Size == TGetArithmeticSize<short int>::Value && Type == TGetArithmeticType<short int>::Value)
        {
            return (short int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<unsigned short int>::Value && Size == TGetArithmeticSize<unsigned short int>::Value && Type == TGetArithmeticType<unsigned short int>::Value)
        {
            return (unsigned short int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<int>::Value && Size == TGetArithmeticSize<int>::Value && Type == TGetArithmeticType<int>::Value)
        {
            return (int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<unsigned int>::Value && Size == TGetArithmeticSize<unsigned int>::Value && Type == TGetArithmeticType<unsigned int>::Value)
        {
            return (unsigned int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<long int>::Value && Size == TGetArithmeticSize<long int>::Value && Type == TGetArithmeticType<long int>::Value)
        {
            return (long int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<unsigned long int>::Value && Size == TGetArithmeticSize<unsigned long int>::Value && Type == TGetArithmeticType<unsigned long int>::Value)
        {
            return (unsigned long int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<long long int>::Value && Size == TGetArithmeticSize<long long int>::Value && Type == TGetArithmeticType<long long int>::Value)
        {
            return (long long int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<unsigned long long int>::Value && Size == TGetArithmeticSize<unsigned long long int>::Value && Type == TGetArithmeticType<unsigned long long int>::Value)
        {
            return (unsigned long long int){};
        }
        else if constexpr (Sign == TGetArithmeticSign<float>::Value && Size == TGetArithmeticSize<float>::Value && Type == TGetArithmeticType<float>::Value)
        {
            return (float){};
        }
        else if constexpr (Sign == TGetArithmeticSign<double>::Value && Size == TGetArithmeticSize<double>::Value && Type == TGetArithmeticType<double>::Value)
        {
            return (double){};
        }
        else if constexpr (Sign == TGetArithmeticSign<long double>::Value && Size == TGetArithmeticSize<long double>::Value && Type == TGetArithmeticType<long double>::Value)
        {
            return (long double){};
        }
        else
        {
            return (NullptrType){};
        }
    }
}
// namespace Aiva::MinLib
