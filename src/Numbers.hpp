#pragma once

#include "Templates.hpp"


#define AIVA_MAKE_NUMBER(name, type, sign, size) \
    using name = Aiva::Templates::NumberIdentity_t<Aiva::Templates::NumberType::type, Aiva::Templates::NumberSign::sign, Aiva::Templates::NumberSize::size>; \

#define AIVA_MAKE_NUMBER_PTR(name, type, sign) \
    using name = Aiva::Templates::NumberIdentity_t<Aiva::Templates::NumberType::type, Aiva::Templates::NumberSign::sign, Aiva::Templates::GetNumberSize<void*>()>; \

namespace Aiva
{
    AIVA_MAKE_NUMBER(int8_t, Int, Sign, Byte1);
    AIVA_MAKE_NUMBER(int16_t, Int, Sign, Byte2);
    AIVA_MAKE_NUMBER(int32_t, Int, Sign, Byte4);
    AIVA_MAKE_NUMBER(int64_t, Int, Sign, Byte8);
    AIVA_MAKE_NUMBER(uint8_t, Int, None, Byte1);
    AIVA_MAKE_NUMBER(uint16_t, Int, None, Byte2);
    AIVA_MAKE_NUMBER(uint32_t, Int, None, Byte4);
    AIVA_MAKE_NUMBER(uint64_t, Int, None, Byte8);
    AIVA_MAKE_NUMBER(float32_t, Flt, None, Byte4);
    AIVA_MAKE_NUMBER(float64_t, Flt, None, Byte8);

    AIVA_MAKE_NUMBER_PTR(intptr_t, Int, Sign);
    AIVA_MAKE_NUMBER_PTR(uintptr_t, Int, None);

    enum class byte_t : uint8_t {};
    using size_t = decltype(sizeof(byte_t));

    using nullptr_t = decltype(nullptr);
}
// namespace Aiva


#undef AIVA_MAKE_NUMBER_PTR
#undef AIVA_MAKE_NUMBER
