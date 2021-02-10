#pragma once

#include "Engine/Core/Core.h"

extern BIGOS::Application* BIGOS::CreateApplication();

int main(int argc, char** argv)
{
	auto app = BIGOS::CreateApplication();
	app->Run();
	app->ShutDown();
	delete app;

	return 0;
}