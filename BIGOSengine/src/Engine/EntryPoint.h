#pragma once

extern BIGOS::Application* BIGOS::CreateApplication();

int main(int argc, char** argv)
{
	auto app = BIGOS::CreateApplication();
	app->Run();
	delete app;

	return 0;
}