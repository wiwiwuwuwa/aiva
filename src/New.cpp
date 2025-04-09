#include "New.hpp"
#include "AllocatorBase.hpp"
#include "Ensures.hpp"
#include "Memory.hpp"


using namespace Aiva;
using namespace Aiva::Memory;


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
