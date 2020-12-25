#pragma once

#include "Engine/Core/Core.h"



extern BIGOS::Application* BIGOS::CreateApplication();

int main(int argc, char** argv)
{
	bool a = true;
	bool b = false;

	auto app = BIGOS::CreateApplication();
	app->Init();
	app->Run();
	app->ShutDown();
	delete app;

	return 0;
}