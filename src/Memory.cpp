#include "Memory.hpp"
#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "RawObject.hpp"
#include "Span.hpp"
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

        Span<byte_t> Alloc(size_t const size) const override;
        nullptr_t Free(Span<byte_t> span) const override;

    private:
        void* m_heap{};
    };

    struct System final
    {
        HeapAllocator m_heapAlloc{};
    };
}
// namespace


static RawObject<System> GSystemObject{};
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


Span<byte_t> HeapAllocator::Alloc(size_t const size) const
{
    auto const data = WinApi::HeapAlloc(m_heap, {}, size);
    if (!data)
        CheckNoEntry();

    return Span<byte_t>{ size, *reinterpret_cast<byte_t*>(data) };
}


nullptr_t HeapAllocator::Free(Span<byte_t> span) const
{
    if (!WinApi::HeapFree(m_heap, {}, &span.GetData()))
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
