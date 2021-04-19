#pragma once
#include "Engine/Core/Core.h"

namespace BIGOS {

	struct SystemMemoryInfo
	{
		uint64_t availablePhysicalMemory;
		uint64_t totalPhysicalMemory;

		uint64_t availableVirtualMemory;
		uint64_t totalVirtualMemory;

		void Log();
	};

	struct MemoryStats
	{
		uint64_t totalAllocated;
		uint64_t totalFreed;
		uint64_t currentUsed;
		uint64_t totalAllocations;

		MemoryStats()
			: totalAllocated(0), totalFreed(0), currentUsed(0), totalAllocations(0)
		{
		}
	};

	class MemoryManager
	{
	public:
		MemoryManager();

		static void Init();
		static void Shutdown();

		SystemMemoryInfo GetSystemInfo();
		static std::string BytesToString(uint64_t bytes);

		static MemoryManager* Get();
		inline MemoryStats GetMemoryStats() const { return m_MemoryStats; }
	private:
		static MemoryManager* s_Instance;
	public:
		MemoryStats m_MemoryStats;
	};

}