#include "memory.hpp"
#include "ensures.hpp"
#include "winapi.hpp"


using namespace aiva;
using namespace aiva::Memory;


namespace
{
    class HeapAllocator final : public AllocatorBase
    {
    public:
        HeapAllocator();
        ~HeapAllocator() override;

        byte_t& Alloc(size_t const size) const override;
        byte_t& Realloc(byte_t& data, size_t const size) const override;
        decltype(nullptr) Free(byte_t& data) const override;

    private:
        void* m_heap;
    };
}
// namespace


static bool GInitialized{};
static MemoryAsObject<HeapAllocator> GHeapAllocator{};


HeapAllocator::HeapAllocator()
{
    m_heap = winapi::HeapCreate({}, {}, {});
    if (!m_heap)
        CheckNoEntryMsg("'winapi::HeapCreate' failed");
}


HeapAllocator::~HeapAllocator()
{
    if (!winapi::HeapDestroy(m_heap))
        CheckNoEntryMsg("'winapi::HeapDestroy' failed");
}


byte_t& HeapAllocator::Alloc(size_t const size) const
{
    auto const data = winapi::HeapAlloc(m_heap, {}, size);
    if (!data)
        CheckNoEntryMsg("'winapi::HeapAlloc' failed");

    return *reinterpret_cast<byte_t*>(data);
}


byte_t& HeapAllocator::Realloc(byte_t& data, size_t const size) const
{
    auto const newData = winapi::HeapReAlloc(m_heap, {}, &data, size);
    if (!newData)
        CheckNoEntryMsg("'winapi::HeapReAlloc' failed");

    return *reinterpret_cast<byte_t*>(newData);
}


decltype(nullptr) HeapAllocator::Free(byte_t& data) const
{
    if (!winapi::HeapFree(m_heap, {}, &data))
        CheckNoEntryMsg("'winapi::HeapFree' failed");

    return nullptr;
}


byte_t& AllocatorBase::Alloc(size_t const) const
{
    CheckNoEntryMsg("Not implemented");
}


byte_t& AllocatorBase::Realloc(byte_t&, size_t const) const
{
    CheckNoEntryMsg("Not implemented");
}


decltype(nullptr) AllocatorBase::Free(byte_t&) const
{
    CheckNoEntryMsg("Not implemented");
}


void Memory::InitSystem()
{
    if (GInitialized)
        CheckNoEntryMsg("Memory system is already initialized");

    GHeapAllocator.Construct();
    GInitialized = true;
}


void Memory::ShutSystem()
{
    if (!GInitialized)
        CheckNoEntryMsg("Memory system is not initialized");

    GHeapAllocator.Destruct();
    GInitialized = false;
}


AllocatorBase const& Memory::GetHeapAlloc()
{
    if (!GInitialized)
        CheckNoEntryMsg("Memory system is not initialized");

    return GHeapAllocator.GetObject();
}


void* operator new(size_t const size)
{
    if (size <= 0)
        CheckNoEntryMsg("'size' is not valid");

    return &GetHeapAlloc().Alloc(size);
}


void* operator new(size_t const size, void *const ptr)
{
    if (size <= 0)
        CheckNoEntryMsg("'size' is not valid");

    return ptr;
}


void operator delete(void *const ptr, size_t const size)
{
    if (!ptr)
        CheckNoEntryMsg("'ptr' is not valid");

    if (size <= 0)
        CheckNoEntryMsg("'size' is not valid");

    GetHeapAlloc().Free(*reinterpret_cast<byte_t*>(ptr));
}
