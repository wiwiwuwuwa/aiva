#pragma once
#include "array.hpp"
#include "ensures.hpp"


namespace aiva
{
    template <typename TType>
    Array<TType>::Array(size_t const size, Memory::Allocator const& allocator)
        : m_allocator{ allocator }
    {
        if (size <= 0)
            CheckNoEntryMsg("'size' is not valid");

        m_span = allocator.CreateArray<TType>(size);
    }


    template <typename TType>
    Array<TType>::~Array()
    {
        m_allocator.DeleteArray(m_span);
    }
}
// namespace aiva
