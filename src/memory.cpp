#include "Memory.hpp"
#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "MemoryAsObject.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Memory;


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
    m_heap = WinApi::HeapCreate({}, {}, {});
    if (!m_heap)
        CheckNoEntry();
}


HeapAllocator::~HeapAllocator()
{
    if (!WinApi::HeapDestroy(m_heap))
        CheckNoEntry();
}


byte_t& HeapAllocator::Alloc(size_t const size) const
{
    auto const data = WinApi::HeapAlloc(m_heap, {}, size);
    if (!data)
        CheckNoEntry();

    return *reinterpret_cast<byte_t*>(data);
}


byte_t& HeapAllocator::Realloc(byte_t& data, size_t const size) const
{
    auto const newData = WinApi::HeapReAlloc(m_heap, {}, &data, size);
    if (!newData)
        CheckNoEntry();

    return *reinterpret_cast<byte_t*>(newData);
}


decltype(nullptr) HeapAllocator::Free(byte_t& data) const
{
    if (!WinApi::HeapFree(m_heap, {}, &data))
        CheckNoEntry();

    return nullptr;
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
