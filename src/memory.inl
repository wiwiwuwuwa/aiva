#pragma once
#include "memory.hpp"
#include "ensures.hpp"
#include "templates.hpp"


namespace aiva::Memory
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
            CheckNoEntryMsg("'size' is not valid");
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
        if (index < 0 || index >= m_size)
            CheckNoEntryMsg("'index' is not valid");

        if (!m_data)
            CheckNoEntryMsg("'m_data' is not valid");

        return m_data[index];
    }


    template <typename TType>
    constexpr TType& Span<TType>::GetData() const
    {
        if (!m_data)
            CheckNoEntryMsg("'m_data' is not valid");

        return *m_data;
    }


    template <typename TType>
    constexpr size_t Span<TType>::GetSize() const
    {
        return m_size;
    }


    template <typename TType, typename... TArgs>
    TType& Allocator::Create(TArgs&&... args) const
    {
        TType& data = reinterpret_cast<TType&>(Alloc(sizeof(TType)));
        data = TType{ Forward<TArgs>(args)... };

        return data;
    }


    template <typename TType, typename... TArgs>
    Span<TType> Allocator::CreateArray(size_t const size, TArgs&&... args) const
    {
        if (size <= 0)
            CheckNoEntryMsg("'size' is not valid");

        TType& data = reinterpret_cast<TType&>(Alloc(sizeof(TType) * size));

        for (auto i = size_t{}; i < size; i++)
            (&data)[i] = TType{ Forward<TArgs>(args)... };

        return Span<TType>{ size, data };
    }


    template <typename TType>
    decltype(nullptr) Allocator::Delete(TType& data) const
    {
        data.~TType();
        return Free(reinterpret_cast<byte_t&>(data));
    }


    template <typename TType>
    decltype(nullptr) Allocator::DeleteArray(Span<TType> const& data) const
    {
        if (!data)
            CheckNoEntryMsg("'data' is not valid");

        for (auto i = data.GetSize(); i > size_t{}; i--)
            data[i - 1].~TType();

        return Free(reinterpret_cast<byte_t&>(data.GetData()));
    }
}
// namespace aiva::Memory
