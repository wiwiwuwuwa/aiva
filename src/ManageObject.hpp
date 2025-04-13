#pragma once

#include "Numbers.hpp"


namespace Aiva
{
    template <typename TType>
    class ManageObject final
    {
    public:
        template <typename... TArgs>
        constexpr void Construct(TArgs&&... args);
        constexpr void Destruct();

        constexpr TType& GetObject();
        constexpr TType const& GetObject() const;

        constexpr operator bool() const;
        constexpr TType& operator*();
        constexpr TType const& operator*() const;
        constexpr TType* operator->();
        constexpr TType const* operator->() const;

    private:
        alignas(alignof(TType)) byte_t m_object[sizeof(TType)];
        TType* m_ptr{};
    };
}
// namespace Aiva


#include "ManageObject.inl"
