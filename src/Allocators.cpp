#include "Allocators.hpp"

#include "Console.hpp"
#include "CstrView.hpp"
#include "Process.hpp"


using namespace Aiva;


Span<byte_t> IAllocator::Alloc(size_t const) const
{
    CheckNoEntry();
}


nullptr_t IAllocator::Free(Span<byte_t> const&) const
{
    CheckNoEntry();
}
