#include "bgspch.h"
#include "Engine/System/MemoryManager.h"


namespace BIGOS {

	MemoryManager* MemoryManager::s_Instance = nullptr;

	void SystemMemoryInfo::Log()
	{
		std::string apm, tpm, avm, tvm;

		uint8_t gb = 1024 * 1024 * 1024;
		uint8_t mb = 1024 * 1024;
		uint8_t kb = 1024;

		apm = MemoryManager::BytesToString(availablePhysicalMemory);
		tpm = MemoryManager::BytesToString(totalPhysicalMemory);
		avm = MemoryManager::BytesToString(availableVirtualMemory);
		tvm = MemoryManager::BytesToString(totalVirtualMemory);

		BGS_CORE_INFO("Memory Info:");
		BGS_CORE_INFO("\tPhysical Memory (Avail/Total): {0} / {1}", apm, tpm);
		BGS_CORE_INFO("\tVirtual Memory  (Avail/Total): {0} / {1}", avm, tvm);
	}

	MemoryManager::MemoryManager()
	{
	}

	void MemoryManager::Init()
	{
	}

	void MemoryManager::Shutdown()
	{
		delete s_Instance;
	}

	SystemMemoryInfo MemoryManager::GetSystemInfo()
	{
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&status);

		SystemMemoryInfo result =
		{
			(uint64_t)status.ullAvailPhys,
			(uint64_t)status.ullTotalPhys,

			(uint64_t)status.ullAvailVirtual,
			(uint64_t)status.ullTotalVirtual
		};
		return result;
	}

	std::string MemoryManager::BytesToString(uint64_t bytes)
	{
		static const float gb = 1024 * 1024 * 1024;
		static const float mb = 1024 * 1024;
		static const float kb = 1024;

		std::ostringstream result;
		if (bytes > gb)
			result << std::fixed << std::setprecision(2) << (bytes / gb) << " GB";
		else if (bytes > mb)
			result << std::fixed << std::setprecision(2) << (bytes / mb) << " MB";
		else if (bytes > kb)
			result << std::fixed << std::setprecision(2) << (bytes / kb) << " KB";
		else
			result << std::fixed << std::setprecision(2) << ((float)bytes) << " bytes";

		return result.str();
	}

	MemoryManager* MemoryManager::Get()
	{
		if (!s_Instance)
		{
			s_Instance = (MemoryManager*)malloc(sizeof(MemoryManager));
			s_Instance = new(s_Instance) MemoryManager();
		}

		return s_Instance;
	}

}