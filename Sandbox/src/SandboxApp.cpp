#include <BIGOS.h>

class Sandbox : public BIGOS::Application
{
public:
	Sandbox()
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