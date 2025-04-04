#include "memory.hpp"
#include "winapi.hpp"

namespace
{
    using namespace aiva;

    void* GHeapPtr{};
    MemoryAllocator GHeapMemoryAlloc{};

    void* AllocateHeapMemory(size_t const size)
    {
        auto const ptr = winapi::HeapAlloc(GHeapPtr, {}, size);
        if (!ptr)
            winapi::ExitProcess(1);

        return ptr;
    }

    void* ReallocateHeapMemory(void* const ptr, size_t const size)
    {
        auto const newPtr = winapi::HeapReAlloc(GHeapPtr, {}, ptr, size);
        if (!newPtr)
            winapi::ExitProcess(1);

        return newPtr;
    }

    void FreeHeapMemory(void* const ptr)
    {
        if (!winapi::HeapFree(GHeapPtr, {}, ptr))
            winapi::ExitProcess(winapi::GetLastError());
    }
}
// namespace

namespace aiva
{
    void InitMemorySystem()
    {
        GHeapPtr = winapi::HeapCreate({}, {}, {});
        if (!GHeapPtr)
            winapi::ExitProcess(winapi::GetLastError());

        GHeapMemoryAlloc.malloc = AllocateHeapMemory;
        GHeapMemoryAlloc.realloc = ReallocateHeapMemory;
        GHeapMemoryAlloc.free = FreeHeapMemory;
    }

    void ShutMemorySystem()
    {
        GHeapMemoryAlloc.free = {};
        GHeapMemoryAlloc.realloc = {};
        GHeapMemoryAlloc.malloc = {};

        if (!winapi::HeapDestroy(GHeapPtr))
            winapi::ExitProcess(winapi::GetLastError());
        GHeapPtr = {};
    }

    const MemoryAllocator& GetHeapMemoryAlloc()
    {
        return GHeapMemoryAlloc;
    }
}
// namespace aiva
