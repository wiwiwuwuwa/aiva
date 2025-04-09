#pragma once
#include "Numbers.hpp"


namespace Aiva
{
    template <typename TType>
    class MemoryAsObject final
    {
    public:
        template <typename... TArgs>
        constexpr void Construct(TArgs&&... args);
        constexpr void Destruct();

        constexpr TType& GetObject();
        constexpr TType const& GetObject() const;

    private:
        alignas(alignof(TType)) byte_t m_object[sizeof(TType)];
    };
}
// namespace Aiva


#include "MemoryAsObject.inl"
