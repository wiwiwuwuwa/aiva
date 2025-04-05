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


    class AllocatorBase
    {
    public:
        virtual byte_t& Alloc(size_t const size) const;
        virtual byte_t& Realloc(byte_t& data, size_t const size) const;
        virtual decltype(nullptr) Free(byte_t& data) const;

        template <typename TType, typename... TArgs>
        TType& Create(TArgs&&... args) const;
        template <typename TType, typename... TArgs>
        Span<TType> CreateArray(size_t const size, TArgs&&... args) const;
        template <typename TType>
        decltype(nullptr) Delete(TType& data) const;
        template <typename TType>
        decltype(nullptr) DeleteArray(Span<TType> const& data) const;

    protected:
        virtual ~AllocatorBase() = default;
    };


    template <typename TType>
    class MemoryAsObject final
    {
    public:
        template <typename... TArgs>
        constexpr void Construct(TArgs&&... args);
        constexpr void Destruct();

        constexpr TType& GetObject();
        constexpr TType const& GetObject() const;

    private:
        alignas(alignof(TType)) byte_t m_object[sizeof(TType)];
    };


    void InitSystem();
    void ShutSystem();

    AllocatorBase const& GetHeapAlloc();
}
// namespace aiva::Memory


void* operator new(size_t const size);
void* operator new(size_t const size, void *const ptr);
void operator delete(void *const ptr, size_t const size);


#include "memory.inl"
