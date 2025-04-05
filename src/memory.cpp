#include "memory.hpp"
#include "ensures.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Memory;


static void* GHeapPtr{};
static Allocator GHeapAlloc{};


static byte_t& AllocateHeapMemory(size_t const size)
{
    auto const data = winapi::HeapAlloc(GHeapPtr, {}, size);
    if (!data)
        CheckNoEntryMsg("'winapi::HeapAlloc' failed");

    return *reinterpret_cast<byte_t*>(data);
}


static byte_t& ReallocateHeapMemory(byte_t& data, size_t const size)
{
    auto const newData = winapi::HeapReAlloc(GHeapPtr, {}, &data, size);
    if (!newData)
        CheckNoEntryMsg("'winapi::HeapReAlloc' failed");

    return *reinterpret_cast<byte_t*>(newData);
}


static decltype(nullptr) FreeHeapMemory(byte_t& data)
{
    if (!winapi::HeapFree(GHeapPtr, {}, &data))
        CheckNoEntryMsg("'winapi::HeapFree' failed");

    return nullptr;
}


byte_t& Allocator::Alloc(size_t const size) const
{
    if (!m_alloc)
        CheckNoEntryMsg("'m_alloc' is not valid");

    return m_alloc(size);
}


byte_t& Allocator::Realloc(byte_t& data, size_t const size) const
{
    if (!m_realloc)
        CheckNoEntryMsg("'m_realloc' is not valid");

    return m_realloc(data, size);
}


decltype(nullptr) Allocator::Free(byte_t& data) const
{
    if (!m_free)
        CheckNoEntryMsg("'m_free' is not valid");

    return m_free(data);
}


void Memory::InitSystem()
{
    GHeapPtr = winapi::HeapCreate({}, {}, {});
    if (!GHeapPtr)
        CheckNoEntryMsg("'winapi::HeapCreate' failed");

    GHeapAlloc.m_alloc = AllocateHeapMemory;
    GHeapAlloc.m_realloc = ReallocateHeapMemory;
    GHeapAlloc.m_free = FreeHeapMemory;
}


void Memory::ShutSystem()
{
    GHeapAlloc.m_free = {};
    GHeapAlloc.m_realloc = {};
    GHeapAlloc.m_alloc = {};

    if (!winapi::HeapDestroy(GHeapPtr))
        CheckNoEntryMsg("'winapi::HeapDestroy' failed");
    GHeapPtr = {};
}


const Allocator& Memory::GetHeapAlloc()
{
    return GHeapAlloc;
}
