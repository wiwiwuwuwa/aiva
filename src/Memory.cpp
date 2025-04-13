#include "Memory.hpp"
#include "Allocators.hpp"
#include "Ensures.hpp"
#include "ManageObject.hpp"
#include "Span.hpp"
#include "SpinLock.hpp"
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

        HeapAllocator(HeapAllocator const&) = delete;
        HeapAllocator& operator=(HeapAllocator const&) = delete;
        HeapAllocator(HeapAllocator&&) = delete;
        HeapAllocator& operator=(HeapAllocator&&) = delete;

        Span<byte_t> Alloc(size_t const size) const override;
        nullptr_t Free(Span<byte_t> span) const override;

    public:
        void* m_heap;
    };
}
// namespace


static SpinLock GHeapAllocLock{};
static ManageObject<HeapAllocator> GHeapAlloc{};


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
    m_heap = {};
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

    return {};
}


void Memory::InitSystem()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };
    GHeapAlloc.Construct();
}


void Memory::ShutSystem()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };
    GHeapAlloc.Destruct();
}


AllocatorBase const& Memory::GetHeapAlloc()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };
    return *GHeapAlloc;
}
