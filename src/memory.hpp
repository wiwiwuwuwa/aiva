#pragma once
#include "numbers.hpp"


namespace aiva::Memory
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


    using AllocFunc_t = byte_t& (*)(size_t);
    using ReallocFunc_t = byte_t& (*)(byte_t&, size_t);
    using FreeFunc_t = decltype(nullptr) (*)(byte_t&);


    struct Allocator final
    {
        byte_t& Alloc(size_t const size) const;
        byte_t& Realloc(byte_t& data, size_t const size) const;
        decltype(nullptr) Free(byte_t& data) const;

        template <typename TType, typename... TArgs>
        TType& Create(TArgs&&... args) const;
        template <typename TType, typename... TArgs>
        Span<TType> CreateArray(size_t const size, TArgs&&... args) const;
        template <typename TType>
        decltype(nullptr) Delete(TType& data) const;
        template <typename TType>
        decltype(nullptr) DeleteArray(Span<TType> const& data) const;

        AllocFunc_t m_alloc{};
        ReallocFunc_t m_realloc{};
        FreeFunc_t m_free{};
    };


    void InitSystem();
    void ShutSystem();

    const Allocator& GetHeapAlloc();
}
// namespace aiva::Memory


#include "memory.inl"
