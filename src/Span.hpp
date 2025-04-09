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

        constexpr operator bool() const;
        constexpr TType& operator[](size_t const index) const;
        constexpr TType& GetData() const;
        constexpr size_t GetSize() const;

    private:
        size_t m_size;
        TType* m_data;
    };
}
// namespace Aiva


#include "Span.inl"
