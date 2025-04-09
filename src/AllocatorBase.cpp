#include "AllocatorBase.hpp"


using namespace Aiva;


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
