#include "bgspch.h"
#include "Engine/Core/Core.h"
#include "Engine/System/System.h"
#include "Engine/Utils/Logger.h"

namespace BIGOS {

	SystemInfo System::s_SystemInfo;

	void System::Init()
	{
		
		MemoryManager::Init();

		s_SystemInfo.memoryInfo = MemoryManager::Get()->GetSystemInfo();
		BIGOS::Logger::Init();
		BGS_CORE_INFO("Initialising system...");
		LogSystemInfo();
	}

	void System::Shutdown()
	{
		BGS_CORE_INFO("Shuting down system...");
		BIGOS::Logger::Shutdown();
	}

	void System::LogSystemInfo()
	{
		BGS_CORE_INFO("--------------------");
		BGS_CORE_INFO(" System Information ");
		BGS_CORE_INFO("--------------------");
		s_SystemInfo.memoryInfo.Log();
	}

}