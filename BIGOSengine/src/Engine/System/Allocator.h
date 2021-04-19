#pragma once
#include "Engine/Core/Core.h"

namespace BIGOS {

	class Allocator
	{
	public:
		static void* Allocate(size_t size);
		static void* AllocateDebug(size_t size, const char* file, uint32_t line);
	
		static void Free(void* block, size_t size);
		static void FreeDebug(void* block, size_t size, const char* file, uint32_t line);
	};
}