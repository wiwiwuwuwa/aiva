#include "Memory.hpp"

#include "Allocators.hpp"
#include "Console.hpp"
#include "CstrView.hpp"
#include "Ensures.hpp"
#include "ManageObject.hpp"
#include "NonCopyable.hpp"
#include "Process.hpp"
#include "Span.hpp"
#include "SpinLock.hpp"
#include "WinApi.hpp"


using namespace Aiva;
using namespace Aiva::Memory;


namespace
{
    class HeapAllocator final : public NonCopyable, public AllocatorBase
    {
    public:
        HeapAllocator();
        ~HeapAllocator() override;

        Span<byte_t> Alloc(size_t const size) const override;
        nullptr_t Free(Span<byte_t> const& span) const override;

    private:
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
    m_heap = nullptr;
}


Span<byte_t> HeapAllocator::Alloc(size_t const size) const
{
    auto const data = WinApi::HeapAlloc(m_heap, {}, size);
    if (!data)
        CheckNoEntry();

    return Span<byte_t>{ size, *reinterpret_cast<byte_t*>(data) };
}


nullptr_t HeapAllocator::Free(Span<byte_t> const& span) const
{
    if (!WinApi::HeapFree(m_heap, {}, span.GetDataPtr()))
        CheckNoEntry();

    return {};
}


void Memory::InitSystem()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };

    if (GHeapAlloc)
        CheckNoEntry();

    GHeapAlloc.Construct();
}


void Memory::ShutSystem()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };

    if (!GHeapAlloc)
        CheckNoEntry();

    GHeapAlloc.Destruct();
}


AllocatorBase const& Memory::GetHeapAlloc()
{
    SpinLockScope_t const lockScope{ GHeapAllocLock };

    if (!GHeapAlloc)
        CheckNoEntry();

    return *GHeapAlloc;
}
