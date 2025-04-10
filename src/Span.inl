#pragma once
#include "Span.hpp"
#include "Ensures.hpp"


namespace Aiva
{
    template <typename TType>
    constexpr Span<TType>::Span()
        : m_size{ 0 }, m_data{ nullptr }
    {

    }

    template <typename TType>
    constexpr Span<TType>::Span(size_t const size, TType& data)
        : m_size{ size }, m_data{ &data }
    {
        if (size <= 0)
            CheckNoEntry();
    }


    template <typename TType>
    constexpr Span<TType>::Span(TType& data)
        : m_size{ 1 }, m_data{ &data }
    {

    }


    template <typename TType>
    constexpr Span<TType>::Span(nullptr_t const)
        : m_size{ 0 }, m_data{ nullptr }
    {

    }


    template <typename TType>
    constexpr Span<TType>::operator bool() const
    {
        return m_size > 0 && m_data;
    }


    template <typename TType>
    constexpr TType& Span<TType>::operator[](size_t const index) const
    {
        if (!(*this))
            CheckNoEntry();

        return m_data[index];
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetData() const
    {
        if (!(*this))
            CheckNoEntry();

        return *m_data;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const
    {
        return m_size;
    }


    template <typename TDst, typename TSrc>
    constexpr Span<TDst> CastSpan(Span<TSrc> const span)
    {
        if (!span)
            return {};

        auto const bytesCount = sizeof(TSrc) * span.GetSize();
        if (bytesCount % sizeof(TDst))
            CheckNoEntry();

        auto const dataAddr = reinterpret_cast<uintptr_t>(&span.GetData());
        if (dataAddr % alignof(TDst))
            CheckNoEntry();

        auto& data = reinterpret_cast<TDst&>(span.GetData());
        auto const size = bytesCount / sizeof(TDst);

        return Span<TDst>{ size, data };
    }
}
// namespace Aiva
