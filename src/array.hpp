#pragma once
#include "memory.hpp"
#include "numbers.hpp"

namespace aiva
{
    template <typename TType>
    class Array final
    {
    public:
        Array(size_t const size, Memory::Allocator const& allocator = Memory::GetHeapAlloc());
        ~Array();

        Array(Array const&) = delete;
        Array& operator=(Array const&) = delete;
        Array(Array&& other) = delete;
        Array& operator=(Array&& other) = delete;

    private:
        Memory::Allocator const& m_allocator;
        Memory::Span<TType> m_span;
    };
}
// namespace aiva


#include "array.inl"
