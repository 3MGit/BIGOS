#pragma once

#include "Allocator.h"

#define bigos_new		new(__FILE__, __LINE__)
#define bigos_delete	delete

#pragma warning(disable : 4595)
#pragma warning(disable : 4005)

inline void* operator new(size_t size)
{
	return BIGOS::Allocator::Allocate(size);
}

inline void* operator new(size_t size, const char* file, uint8_t line)
{
	return BIGOS::Allocator::AllocateDebug(size, file, line);
}

inline void* operator new[](size_t size)
{
	return BIGOS::Allocator::Allocate(size);
}

inline void* operator new[](size_t size, const char* file, uint8_t line)
{
	return BIGOS::Allocator::AllocateDebug(size, file, line);
}

inline void operator delete(void* block, size_t size)
{
	BIGOS::Allocator::Free(block, size);
}

inline void operator delete(void* block, size_t size, const char* file, uint8_t line)
{
	BIGOS::Allocator::FreeDebug(block, size, file, line);
}

inline void operator delete[](void* block, size_t size)
{
	BIGOS::Allocator::Free(block, size);
}

inline void operator delete[](void* block, size_t size, const char* file, uint8_t line)
{
	BIGOS::Allocator::FreeDebug(block, size, file, line);
}

#pragma warning(default : 4595)

