#include "bgspch.h"

#include "Engine\Core\Application.h"
#include "Engine\Core\Logger.h"

#include "Engine/Events/ApplicationEvent.h"

namespace BIGOS {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		:m_Name(name)
	{
	}

	Application::~Application()
	{
	}

	bool Application::Init()
	{
		BGS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::StartUpWindow(WindowProps(m_Name));
		m_Window->SetEventCallback(BGS_BIND_EVENT_FN(Application::OnEvent));

		BGS_CORE_ASSERT(m_Window->Init());

		return true;
	}

	void Application::ShutDown()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BGS_BIND_EVENT_FN(Application::OnWindowClose));
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}