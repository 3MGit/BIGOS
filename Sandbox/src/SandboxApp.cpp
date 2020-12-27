#include <BIGOS.h>
#include <Engine/Core/EntryPoint.h>

#include "TestLayer.h"

class Sandbox : public BIGOS::Application
{
public:
	Sandbox()
		: Application("SandboxApp")
	{
		PushLayer(new TestLayer());
	}

	~Sandbox()
	{

	}
};

BIGOS::Application* BIGOS::CreateApplication()
{
	return new Sandbox();
}