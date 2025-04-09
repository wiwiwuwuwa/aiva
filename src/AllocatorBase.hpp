#pragma once
#include "Numbers.hpp"
#include "Span.hpp"


namespace Aiva
{
    class AllocatorBase
    {
    public:
        virtual byte_t& Alloc(size_t const size) const;
        virtual byte_t& Realloc(byte_t& data, size_t const size) const;
        virtual decltype(nullptr) Free(byte_t& data) const;

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
