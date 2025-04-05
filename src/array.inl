#pragma once
#include "array.hpp"
#include "ensures.hpp"


namespace aiva
{
    template <typename TType>
    Array<TType>::Array(size_t const size, Memory::AllocatorBase const& allocator)
        : m_allocator{ allocator },
          m_span{ allocator.CreateArray<TType>(size) }
    {
        if (size <= 0)
            CheckNoEntryMsg("'size' is not valid");
    }


    template <typename TType>
    Array<TType>::Array(CreateArrayUnsafe const, size_t const size, Memory::AllocatorBase const& allocator)
        : m_allocator{ allocator },
          m_span{ size, reinterpret_cast<TType&>(m_allocator.Alloc(sizeof(TType) * size)) }
    {
        if (size <= 0)
            CheckNoEntryMsg("'size' is not valid");
    }


    template <typename TType>
    Array<TType>::~Array()
    {
        m_allocator.DeleteArray(m_span);
    }


    template <typename TType>
    constexpr TType& Array<TType>::operator[](size_t const index)
    {
        return m_span[index];
    }


    template <typename TType>
    constexpr TType const& Array<TType>::operator[](size_t const index) const
    {
        return m_span[index];
    }


    template <typename TType>
    constexpr size_t Array<TType>::GetSize() const
    {
        return m_span.GetSize();
    }
}
// namespace aiva
