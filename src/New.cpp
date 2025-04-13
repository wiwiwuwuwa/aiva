#include "New.hpp"
#include "Allocators.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"


using namespace Aiva;
using namespace Aiva::Memory;


void* operator new(size_t const size)
{
    auto const spanOfBytes = GetHeapAlloc().Alloc(size);
    return &spanOfBytes.GetData();
}


void* operator new(size_t const, void *const ptr)
{
    return ptr;
}


void operator delete(void *const ptr, size_t const size)
{
    auto const spanOfBytes = Span<byte_t>{ size, *reinterpret_cast<byte_t*>(ptr) };
    GetHeapAlloc().Free(spanOfBytes);
}
