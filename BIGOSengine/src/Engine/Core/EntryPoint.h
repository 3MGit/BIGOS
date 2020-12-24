#pragma once

#include "Engine/Core/Logger.h"

#include <string>

extern BIGOS::Application* BIGOS::CreateApplication();

int main(int argc, char** argv)
{

	std::string message1 = "logg message 1 ...";
	int    nNum = 10;
	BGS_CORE_TRACE(message1);
	BGS_TRACE("Number: %d", nNum);

	BGS_CORE_INFO(message1);
	BGS_INFO("Number: %d", nNum);

	BGS_CORE_WARN(message1);
	BGS_WARN("Number: %d", nNum);

	BGS_CORE_ERROR(message1);
	BGS_ERROR("Number: %d", nNum);

	BGS_CORE_FATAL(message1);
	BGS_FATAL("Number: %d", nNum);

	auto app = BIGOS::CreateApplication();
	app->Run();
	delete app;

	return 0;
}