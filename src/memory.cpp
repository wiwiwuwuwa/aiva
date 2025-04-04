#include "memory.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Memory;


static void* GHeapPtr{};
static Allocator GHeapAlloc{};


static void* AllocateHeapMemory(size_t const size)
{
    auto const ptr = winapi::HeapAlloc(GHeapPtr, {}, size);
    if (!ptr)
        winapi::ExitProcess(1);

    return ptr;
}


static void* ReallocateHeapMemory(void* const ptr, size_t const size)
{
    auto const newPtr = winapi::HeapReAlloc(GHeapPtr, {}, ptr, size);
    if (!newPtr)
        winapi::ExitProcess(1);

    return newPtr;
}


static void FreeHeapMemory(void* const ptr)
{
    if (!winapi::HeapFree(GHeapPtr, {}, ptr))
        winapi::ExitProcess(winapi::GetLastError());
}


void Memory::InitSystem()
{
    GHeapPtr = winapi::HeapCreate({}, {}, {});
    if (!GHeapPtr)
        winapi::ExitProcess(winapi::GetLastError());

    GHeapAlloc.alloc = AllocateHeapMemory;
    GHeapAlloc.realloc = ReallocateHeapMemory;
    GHeapAlloc.free = FreeHeapMemory;
}


void Memory::ShutSystem()
{
    GHeapAlloc.free = {};
    GHeapAlloc.realloc = {};
    GHeapAlloc.alloc = {};

    if (!winapi::HeapDestroy(GHeapPtr))
        winapi::ExitProcess(winapi::GetLastError());
    GHeapPtr = {};
}


const Allocator& GetHeapAlloc()
{
    return GHeapAlloc;
}
