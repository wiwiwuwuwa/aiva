#pragma once
#include "numbers.hpp"


namespace aiva::Memory
{
    using AllocFunc_t = void* (*)(size_t);
    using ReallocFunc_t = void* (*)(void*, size_t);
    using FreeFunc_t = void (*)(void*);


    struct Allocator final
    {
        AllocFunc_t alloc{};
        ReallocFunc_t realloc{};
        FreeFunc_t free{};
    };


    void InitSystem();
    void ShutSystem();


    const Allocator& GetHeapAlloc();
}
// namespace aiva
