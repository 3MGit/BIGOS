#include "bgspch.h"

#include "Engine/Utils/Logger.h"
#include "Engine/Utils/TimeUtils.h"

namespace BIGOS {

	Logger* Logger::s_Instance = NULL;

	Logger* Logger::Get()
	{
		if (s_Instance == NULL) {
			s_Instance = new Logger();
		}
		return s_Instance;
	}

	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::Log(LogType type, LogLevel lvl, const std::string& message)
	{
		std::ostringstream oss;
		oss << "[" << Utils::CurrentDateTime() << "] ";
		switch (type)
		{
		case BIGOS::LogType::CLIENT:
			break;
		case BIGOS::LogType::CORE:
			oss << "BIGOS:";
			break;
		default:
			break;
		}
		switch (lvl)
		{
		case BIGOS::LogLevel::TRACE:
			oss << "TRACE:\t";
			break;
		case BIGOS::LogLevel::INFO:
			oss << "INFO:\t";
			break;
		case BIGOS::LogLevel::WARN:
			oss << "WARN:\t";
			break;
		case BIGOS::LogLevel::FATAL:
			oss << "FATAL:\t";
			break;
		default:
			break;
		}
		oss << message << "\n";

		std::cout << oss.str();
	}

	void Logger::Log(LogType type, LogLevel lvl, const char* format, ...)
	{
		std::ostringstream oss;
		char* sMessage = NULL;
		int nLength = 0;
		va_list args;
		va_start(args, format);
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf(format, args) + 1;
		sMessage = new char[nLength];
		vsprintf_s(sMessage, nLength, format, args);
		//vsprintf(sMessage, format, args);
		oss << "[" << Utils::CurrentDateTime() << "] ";
		switch (type)
		{
		case BIGOS::LogType::CLIENT:
			break;
		case BIGOS::LogType::CORE:
			oss << "BIGOS:";
			break;
		default:
			break;
		}
		switch (lvl)
		{
		case BIGOS::LogLevel::TRACE:
			oss << "TRACE:\t";
			break;
		case BIGOS::LogLevel::INFO:
			oss << "INFO:\t";
			break;
		case BIGOS::LogLevel::WARN:
			oss << "WARN:\t";
			break;
		case BIGOS::LogLevel::FATAL:
			oss << "FATAL:\t";
			break;
		default:
			break;
		}
		oss << sMessage << "\n";
		va_end(args);	

		std::cout << oss.str();

		delete[] sMessage;
	}


}