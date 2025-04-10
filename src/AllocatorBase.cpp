#include "AllocatorBase.hpp"


using namespace Aiva;


Span<byte_t> AllocatorBase::Alloc(size_t const) const
{
    CheckNoEntry();
}


Span<byte_t> AllocatorBase::Realloc(Span<byte_t>, size_t const) const
{
    CheckNoEntry();
}


decltype(nullptr) AllocatorBase::Free(Span<byte_t>) const
{
    CheckNoEntry();
}
