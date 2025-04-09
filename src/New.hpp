#pragma once
#include "Numbers.hpp"


void* operator new(size_t const size);
void* operator new(size_t const size, void *const ptr);
void operator delete(void *const ptr, size_t const size);
