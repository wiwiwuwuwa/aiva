#pragma once

#include "Numbers.hpp"


namespace Aiva
{
    template <typename TType>
    class Span final
    {
    public:
        constexpr Span();
        constexpr Span(size_t const size, TType& data);
        constexpr Span(TType& data);
        constexpr Span(nullptr_t const);

        constexpr operator bool() const;
        constexpr TType& operator[](size_t const index) const;
        constexpr size_t GetSize() const;
        constexpr TType* GetDataPtr() const;
        constexpr TType& GetDataRef() const;

    public:
        template <size_t TSize>
        constexpr Span(TType (&data)[TSize]);

    private:
        size_t m_size;
        TType* m_data;
    };


    template <typename TDst, typename TSrc>
    constexpr Span<TDst> CastSpan(Span<TSrc> const span);
}
// namespace Aiva


#include "Span.inl"
