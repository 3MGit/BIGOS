#include <BIGOS.h>

class Sandbox : public BIGOS::Application
{
public:
	Sandbox()
		: Application("SandboxApp")
	{

	}

	~Sandbox()
	{

	}
};

BIGOS::Application* BIGOS::CreateApplication()
{
	return new Sandbox();
}