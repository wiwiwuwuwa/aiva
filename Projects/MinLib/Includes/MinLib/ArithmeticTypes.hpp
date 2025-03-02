#pragma once
#include "TGetArithmeticTypename.hpp"
#include "TToArithmeticSize.hpp"

namespace Aiva::MinLib
{
    using int8_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte1, EArithmeticType::Integral>::Type;
    using int16_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte2, EArithmeticType::Integral>::Type;
    using int32_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte4, EArithmeticType::Integral>::Type;
    using int64_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte8, EArithmeticType::Integral>::Type;

    using uint8_t = TGetArithmeticTypename<EArithmeticSign::Unsigned, EArithmeticSize::Byte1, EArithmeticType::Integral>::Type;
    using uint16_t = TGetArithmeticTypename<EArithmeticSign::Unsigned, EArithmeticSize::Byte2, EArithmeticType::Integral>::Type;
    using uint32_t = TGetArithmeticTypename<EArithmeticSign::Unsigned, EArithmeticSize::Byte4, EArithmeticType::Integral>::Type;
    using uint64_t = TGetArithmeticTypename<EArithmeticSign::Unsigned, EArithmeticSize::Byte8, EArithmeticType::Integral>::Type;

    using float32_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte4, EArithmeticType::Floating>::Type;
    using float64_t = TGetArithmeticTypename<EArithmeticSign::Signed, EArithmeticSize::Byte8, EArithmeticType::Floating>::Type;

    using intptr_t = TGetArithmeticTypename<EArithmeticSign::Signed, TToArithmeticSize<sizeof(void*)>::Value, EArithmeticType::Integral>::Type;
    using uintptr_t = TGetArithmeticTypename<EArithmeticSign::Unsigned, TToArithmeticSize<sizeof(void*)>::Value, EArithmeticType::Integral>::Type;
}
// namespace Aiva::MinLib
