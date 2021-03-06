#pragma once
#include "bgspch.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace BIGOS {

	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define BGS_CORE_TRACE(...)    ::BIGOS::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define BGS_CORE_INFO(...)     ::BIGOS::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define BGS_CORE_WARN(...)     ::BIGOS::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define BGS_CORE_ERROR(...)    ::BIGOS::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define BGS_CORE_FATAL(...)	   ::BIGOS::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define BGS_TRACE(...)         ::BIGOS::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define BGS_INFO(...)          ::BIGOS::Logger::GetClientLogger()->info(__VA_ARGS__)
#define BGS_WARN(...)          ::BIGOS::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define BGS_ERROR(...)         ::BIGOS::Logger::GetClientLogger()->error(__VA_ARGS__)
#define BGS_FATAL(...)		   ::BIGOS::Logger::GetClientLogger()->critical(__VA_ARGS__)