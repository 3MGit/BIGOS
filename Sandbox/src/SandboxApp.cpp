#include <BIGOS.h>
#include <Engine/Core/EntryPoint.h>

#include "PBRDemoLayer.h"

class Sandbox : public BIGOS::Application
{
public:
	Sandbox()
		: Application("SandboxApp")
	{
		PushLayer(new PBRDemoLayer());
	}

	~Sandbox()
	{

	}
};

BIGOS::Application* BIGOS::CreateApplication()
{
	return new Sandbox();
}