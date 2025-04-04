#pragma once
#include "memory.hpp"
#include "templates.hpp"


namespace aiva::Memory
{
    template <typename TType>
    Span<TType>::Span(TType *const data, size_t const size)
    {
        if (data && size)
        {
            m_data = data;
            m_size = size;
        }
        else
        {
            m_data = {};
            m_size = {};
        }
    }


    template <typename TType>
    Span<TType>::Span(TType *const data)
    {
        if (data)
        {
            m_data = data;
            m_size = 1;
        }
        else
        {
            m_data = {};
            m_size = {};
        }
    }


    template <typename TType>
    TType *Span<TType>::GetData() const
    {
        return m_data;
    }


    template <typename TType>
    size_t Span<TType>::GetSize() const
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


    template <typename TType>
    decltype(nullptr) Allocator::Delete(TType& data) const
    {
        data.~TType();
        return Free(reinterpret_cast<byte_t&>(data));
    }
}
// namespace aiva::Memory
