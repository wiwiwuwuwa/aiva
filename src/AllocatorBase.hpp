#pragma once
#include "Numbers.hpp"
#include "Span.hpp"


namespace Aiva
{
    class AllocatorBase
    {
    public:
        virtual Span<byte_t> Alloc(size_t const size) const;
        virtual Span<byte_t> Realloc(Span<byte_t> const span, size_t const size) const;
        virtual decltype(nullptr) Free(Span<byte_t> const span) const;

    public:
        template <typename TType, typename... TArgs>
        TType& Create(TArgs&&... args) const;

        template <typename TType, typename... TArgs>
        Span<TType> CreateArray(size_t const size, TArgs&&... args) const;

        template <typename TType>
        decltype(nullptr) Delete(TType& data) const;

        template <typename TType>
        decltype(nullptr) DeleteArray(Span<TType> const& data) const;

    protected:
        virtual ~AllocatorBase() = default;
    };
}
// namespace Aiva


#include "AllocatorBase.inl"
