#pragma once
#include "Engine/System/MemoryManager.h"

namespace BIGOS {

	struct SystemInfo
	{
		SystemMemoryInfo memoryInfo;
	};

	class System
	{
	public:
		static void Init();
		static void Shutdown();

		static SystemInfo GetSystemInfo() { return s_SystemInfo; }
	private:
		static SystemInfo s_SystemInfo;
	private:
		static void LogSystemInfo();
	};

}
