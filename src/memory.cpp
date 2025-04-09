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
        void* m_heap{};
    };

    struct System final
    {
        HeapAllocator m_heapAlloc{};
    };
}
// namespace


static MemoryAsObject<System> GSystemObject{};
static System* GSystem{};


HeapAllocator::HeapAllocator()
{
    m_heap = winapi::HeapCreate({}, {}, {});
    if (!m_heap)
        CheckNoEntry();
}


HeapAllocator::~HeapAllocator()
{
    if (!winapi::HeapDestroy(m_heap))
        CheckNoEntry();
}


byte_t& HeapAllocator::Alloc(size_t const size) const
{
    auto const data = winapi::HeapAlloc(m_heap, {}, size);
    if (!data)
        CheckNoEntry();

    return *reinterpret_cast<byte_t*>(data);
}


byte_t& HeapAllocator::Realloc(byte_t& data, size_t const size) const
{
    auto const newData = winapi::HeapReAlloc(m_heap, {}, &data, size);
    if (!newData)
        CheckNoEntry();

    return *reinterpret_cast<byte_t*>(newData);
}


decltype(nullptr) HeapAllocator::Free(byte_t& data) const
{
    if (!winapi::HeapFree(m_heap, {}, &data))
        CheckNoEntry();

    return nullptr;
}


byte_t& AllocatorBase::Alloc(size_t const) const
{
    CheckNoEntry();
}


byte_t& AllocatorBase::Realloc(byte_t&, size_t const) const
{
    CheckNoEntry();
}


decltype(nullptr) AllocatorBase::Free(byte_t&) const
{
    CheckNoEntry();
}


void Memory::InitSystem()
{
    if (GSystem)
        CheckNoEntry();

    GSystemObject.Construct();
    GSystem = &GSystemObject.GetObject();
}


void Memory::ShutSystem()
{
    if (!GSystem)
        CheckNoEntry();

    GSystem = nullptr;
    GSystemObject.Destruct();
}


AllocatorBase const& Memory::GetHeapAlloc()
{
    if (!GSystem)
        CheckNoEntry();

    return GSystem->m_heapAlloc;
}


void* operator new(size_t const size)
{
    if (size <= 0)
        CheckNoEntry();

    return &GetHeapAlloc().Alloc(size);
}


void* operator new(size_t const size, void *const ptr)
{
    if (size <= 0)
        CheckNoEntry();

    return ptr;
}


void operator delete(void *const ptr, size_t const size)
{
    if (!ptr || size <= 0)
        CheckNoEntry();

    GetHeapAlloc().Free(*reinterpret_cast<byte_t*>(ptr));
}
