#include "bgspch.h"

#include "Engine/Core/App/Application.h"
#include "Engine/Core/Logger.h"


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
		EventManager manager(e);
		manager.Dispatch<WindowCloseEvent>(BGS_BIND_EVENT_FN(Application::OnWindowClose));
		manager.Dispatch<WindowResizeEvent>(BGS_BIND_EVENT_FN(Application::OnWindowResize));
		manager.Dispatch<MouseMovedEvent>(BGS_BIND_EVENT_FN(Application::OnMouseMoved));
		manager.Dispatch<MouseButtonPressedEvent>(BGS_BIND_EVENT_FN(Application::OnMousePressed));
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();

			if (Input::IsKeyPressed(Key::A))
				BGS_CORE_INFO("A");
			if (Input::IsKeyPressed(Key::W))
				BGS_CORE_INFO("W");
			if (Input::IsKeyPressed(Key::D))
				BGS_CORE_INFO("D");
			if (Input::IsKeyPressed(Key::S))
				BGS_CORE_INFO("S");
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		BGS_CORE_TRACE(e.ToString());
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		BGS_CORE_TRACE(e.ToString());
		return true;
	}

	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		BGS_CORE_TRACE(e.ToString());
		return true;
	}

	bool Application::OnMousePressed(MouseButtonPressedEvent& e)
	{
		BGS_CORE_TRACE(e.ToString());
		return true;
	}
}