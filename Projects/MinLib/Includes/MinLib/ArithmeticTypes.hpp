#pragma once
#include <MinLib/GetArithmeticTypename.hpp>
#include <MinLib/ToArithmeticSize.hpp>

namespace Aiva::MinLib
{
    using int8_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte1, ArithmeticType::Integral>::Type;
    using int16_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte2, ArithmeticType::Integral>::Type;
    using int32_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte4, ArithmeticType::Integral>::Type;
    using int64_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte8, ArithmeticType::Integral>::Type;

    using uint8_t = GetArithmeticTypename<ArithmeticSign::Unsigned, ArithmeticSize::Byte1, ArithmeticType::Integral>::Type;
    using uint16_t = GetArithmeticTypename<ArithmeticSign::Unsigned, ArithmeticSize::Byte2, ArithmeticType::Integral>::Type;
    using uint32_t = GetArithmeticTypename<ArithmeticSign::Unsigned, ArithmeticSize::Byte4, ArithmeticType::Integral>::Type;
    using uint64_t = GetArithmeticTypename<ArithmeticSign::Unsigned, ArithmeticSize::Byte8, ArithmeticType::Integral>::Type;

    using float32_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte4, ArithmeticType::Floating>::Type;
    using float64_t = GetArithmeticTypename<ArithmeticSign::Signed, ArithmeticSize::Byte8, ArithmeticType::Floating>::Type;

    using intptr_t = GetArithmeticTypename<ArithmeticSign::Signed, ToArithmeticSize<sizeof(void*)>::Value, ArithmeticType::Integral>::Type;
    using uintptr_t = GetArithmeticTypename<ArithmeticSign::Unsigned, ToArithmeticSize<sizeof(void*)>::Value, ArithmeticType::Integral>::Type;
}
// namespace Aiva::MinLib
