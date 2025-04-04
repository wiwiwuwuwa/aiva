#pragma once
#include "numbers.hpp"

namespace aiva
{
    using MemoryAllocFunc_t = void* (*)(size_t);
    using MemoryReallocFunc_t = void* (*)(void*, size_t);
    using MemoryFreeFunc_t = void (*)(void*);

    struct MemoryAllocator final
    {
        MemoryAllocFunc_t malloc{};
        MemoryReallocFunc_t realloc{};
        MemoryFreeFunc_t free{};
    };

    void InitMemorySystem();
    void ShutMemorySystem();

    const MemoryAllocator& GetHeapMemoryAlloc();
}
// namespace aiva
