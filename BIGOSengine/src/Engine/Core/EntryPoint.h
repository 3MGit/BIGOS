#pragma once

#include "Engine/Core/Core.h"
#include "Engine/System/System.h"

extern BIGOS::Application* BIGOS::CreateApplication();

int main(int argc, char** argv)
{
	BIGOS::System::Init();

	auto app = BIGOS::CreateApplication();
	app->Run();
	app->ShutDown();
	delete app;

	BIGOS::System::Shutdown();

	return 0;
}