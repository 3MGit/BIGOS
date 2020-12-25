#include "bgspch.h"

#include "Engine\Core\Application.h"
#include "Engine\Core\Logger.h"

namespace BIGOS {

	Application::Application(const std::string& name)
		:m_Name(name)
	{
	}

	Application::~Application()
	{
	}

	bool Application::Init()
	{
		m_Window = Window::StartUpWindow(WindowProps(m_Name));

		BGS_CORE_ASSERT(m_Window->Init());

		return true;
	}

	void Application::ShutDown()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

}