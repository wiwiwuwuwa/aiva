#pragma once
#include <MinLib/GetArithmeticSign.hpp>
#include <MinLib/GetArithmeticSize.hpp>
#include <MinLib/GetArithmeticType.hpp>

namespace Aiva::MinLib
{
    template <ArithmeticSign Sign, ArithmeticSize Size, ArithmeticType Type>
    constexpr auto GetDefaultArithmeticTypenameValue()
    {
        if constexpr (Sign == GetArithmeticSign<char>::Value && Size == GetArithmeticSize<char>::Value && Type == GetArithmeticType<char>::Value)
            { return (char){}; }

        else if constexpr (Sign == GetArithmeticSign<signed char>::Value && Size == GetArithmeticSize<signed char>::Value && Type == GetArithmeticType<signed char>::Value)
            { return (signed char){}; }

        else if constexpr (Sign == GetArithmeticSign<unsigned char>::Value && Size == GetArithmeticSize<unsigned char>::Value && Type == GetArithmeticType<unsigned char>::Value)
            { return (unsigned char){}; }

        else if constexpr (Sign == GetArithmeticSign<short int>::Value && Size == GetArithmeticSize<short int>::Value && Type == GetArithmeticType<short int>::Value)
            { return (short int){}; }

        else if constexpr (Sign == GetArithmeticSign<unsigned short int>::Value && Size == GetArithmeticSize<unsigned short int>::Value && Type == GetArithmeticType<unsigned short int>::Value)
            { return (unsigned short int){}; }

        else if constexpr (Sign == GetArithmeticSign<int>::Value && Size == GetArithmeticSize<int>::Value && Type == GetArithmeticType<int>::Value)
            { return (int){}; }

        else if constexpr (Sign == GetArithmeticSign<unsigned int>::Value && Size == GetArithmeticSize<unsigned int>::Value && Type == GetArithmeticType<unsigned int>::Value)
            { return (unsigned int){}; }

        else if constexpr (Sign == GetArithmeticSign<long int>::Value && Size == GetArithmeticSize<long int>::Value && Type == GetArithmeticType<long int>::Value)
            { return (long int){}; }

        else if constexpr (Sign == GetArithmeticSign<unsigned long int>::Value && Size == GetArithmeticSize<unsigned long int>::Value && Type == GetArithmeticType<unsigned long int>::Value)
            { return (unsigned long int){}; }

        else if constexpr (Sign == GetArithmeticSign<long long int>::Value && Size == GetArithmeticSize<long long int>::Value && Type == GetArithmeticType<long long int>::Value)
            { return (long long int){}; }

        else if constexpr (Sign == GetArithmeticSign<unsigned long long int>::Value && Size == GetArithmeticSize<unsigned long long int>::Value && Type == GetArithmeticType<unsigned long long int>::Value)
            { return (unsigned long long int){}; }

        else if constexpr (Sign == GetArithmeticSign<float>::Value && Size == GetArithmeticSize<float>::Value && Type == GetArithmeticType<float>::Value)
            { return (float){}; }

        else if constexpr (Sign == GetArithmeticSign<double>::Value && Size == GetArithmeticSize<double>::Value && Type == GetArithmeticType<double>::Value)
            { return (double){}; }

        else if constexpr (Sign == GetArithmeticSign<long double>::Value && Size == GetArithmeticSize<long double>::Value && Type == GetArithmeticType<long double>::Value)
            { return (long double){}; }
    }
}
// namespace Aiva::MinLib
