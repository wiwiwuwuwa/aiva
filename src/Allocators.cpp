#include "Allocators.hpp"


using namespace Aiva;


Span<byte_t> AllocatorBase::Alloc(size_t const) const
{
    CheckNoEntry();
}


nullptr_t AllocatorBase::Free(Span<byte_t>) const
{
    CheckNoEntry();
}
