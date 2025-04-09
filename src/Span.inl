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
    constexpr Span<TType>::operator bool() const
    {
        return m_size > 0 && m_data;
    }


    template <typename TType>
    constexpr TType& Span<TType>::operator[](size_t const index) const
    {
        if (index < 0 || index >= m_size || !m_data)
            CheckNoEntry();

        return m_data[index];
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetData() const
    {
        if (!m_data)
            CheckNoEntry();

        return *m_data;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const
    {
        return m_size;
    }
}
// namespace Aiva
