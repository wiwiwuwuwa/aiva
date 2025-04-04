#pragma once
#include "numbers.hpp"


namespace aiva::Memory
{
    template <typename TType>
    class Span final
    {
    public:
        Span(TType *const data, size_t const size);
        Span(TType *const data);

        TType* GetData() const;
        size_t GetSize() const;

    private:
        TType *const m_data;
        size_t const m_size;
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
        template <typename TType>
        decltype(nullptr) Delete(TType& data) const;

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
