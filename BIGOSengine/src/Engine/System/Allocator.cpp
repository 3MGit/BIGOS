#include "bgspch.h"
#include "Engine/System/Allocator.h"
#include "Engine/System/MemoryManager.h"
#include "Engine/Core/Core.h"

#include <malloc.h>

namespace BIGOS {

	void* Allocator::Allocate(size_t size)
	{
		BGS_CORE_ASSERT(size < 1024 * 1024 * 1024, "Memory block is to big to allocate!");

		MemoryManager::Get()->m_MemoryStats.totalAllocated += size;
		MemoryManager::Get()->m_MemoryStats.currentUsed += size;
		MemoryManager::Get()->m_MemoryStats.totalAllocations++;

		uint8_t* result = (uint8_t*)malloc(size);

		return result;
	}

	void* Allocator::AllocateDebug(size_t size, const char* file, uint32_t line)
	{
		BGS_CORE_ASSERT(size < 1024 * 1024 * 1024, "Memory block is to big to allocate!");
		if (size > 1024 * 1024)
			BGS_CORE_WARN("Large allocation at {0} : {1}", file, line);

		return Allocate(size);
	}

	void Allocator::Free(void* block, size_t size)
	{
		MemoryManager::Get()->m_MemoryStats.totalFreed += size;
		MemoryManager::Get()->m_MemoryStats.currentUsed -= size;
		free(block);
	}

	void Allocator::FreeDebug(void* block, size_t size, const char* file, uint32_t line)
	{
		Free(block, size);
	}

}