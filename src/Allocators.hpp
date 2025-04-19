#pragma once

#include "Numbers.hpp"
#include "Span.hpp"


namespace Aiva
{
    class AllocatorBase
    {
    public:
        virtual Span<byte_t> Alloc(size_t const size) const;
        virtual nullptr_t Free(Span<byte_t> const& span) const;

    public:
        template <typename TType>
        Span<TType> AllocArray(size_t const size) const;

        template <typename TType>
        nullptr_t FreeArray(Span<TType> const& span) const;

    public:
        template <typename TType, typename... TArgs>
        TType& Create(TArgs&&... args) const;

        template <typename TType, typename... TArgs>
        Span<TType> CreateArray(size_t const size, TArgs&&... args) const;

        template <typename TType>
        nullptr_t Delete(TType& data) const;

        template <typename TType>
        nullptr_t DeleteArray(Span<TType> const& data) const;

    protected:
        virtual ~AllocatorBase() = default;
    };
}
// namespace Aiva


#include "Allocators.inl"
