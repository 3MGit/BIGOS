#pragma once

#include "bgspch.h"
 
namespace BIGOS {

	enum class LogType {CLIENT, CORE};
	enum class LogLevel { TRACE, INFO, WARN, FATAL};

    class Logger 
    {
	public:
		Logger(const Logger&) = delete;

		static Logger* Get();

		void Log(LogType type, LogLevel lvl, const std::string& message);
		void Log(LogType type, LogLevel lvl, const char* format, ...);

	private:
		Logger();
		~Logger();

	private:

		static Logger* s_Instance;
    };

}

// Core log macros
#define BGS_CORE_TRACE(...)    ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CORE,::BIGOS::LogLevel::TRACE, __VA_ARGS__)
#define BGS_CORE_INFO(...)     ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CORE,::BIGOS::LogLevel::INFO, __VA_ARGS__)
#define BGS_CORE_WARN(...)     ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CORE,::BIGOS::LogLevel::WARN, __VA_ARGS__)
#define BGS_CORE_FATAL(...)    ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CORE,::BIGOS::LogLevel::FATAL, __VA_ARGS__)

// Client log macros
#define BGS_TRACE(...)	      ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CLIENT, ::BIGOS::LogLevel::TRACE, __VA_ARGS__)
#define BGS_INFO(...)	      ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CLIENT, ::BIGOS::LogLevel::INFO, __VA_ARGS__)
#define BGS_WARN(...)	      ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CLIENT, ::BIGOS::LogLevel::WARN, __VA_ARGS__)
#define BGS_FATAL(...)	      ::BIGOS::Logger::Get()->Log(::BIGOS::LogType::CLIENT, ::BIGOS::LogLevel::FATAL, __VA_ARGS__)
