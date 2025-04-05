#pragma once
#include "memory.hpp"
#include "numbers.hpp"

namespace aiva
{
    template <typename TType>
    class Array final
    {
    public:
        Array(size_t const size, Memory::AllocatorBase const& allocator = Memory::GetHeapAlloc());
        ~Array();

        constexpr Array(Array const&) = delete;
        constexpr Array& operator=(Array const&) = delete;
        constexpr Array(Array&& other) = delete;
        constexpr Array& operator=(Array&& other) = delete;

        constexpr TType& operator[](size_t const index);
        constexpr TType const& operator[](size_t const index) const;
        constexpr size_t GetSize() const;

    private:
        Memory::AllocatorBase const& m_allocator;
        Memory::Span<TType> m_span;
    };
}
// namespace aiva


#include "array.inl"
