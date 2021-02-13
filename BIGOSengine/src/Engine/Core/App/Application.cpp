#include "bgspch.h"

#include "Engine/Core/Core.h"
#include "Engine/Core/App/Application.h"

#include "Engine/Renderer/Renderer.h"

namespace BIGOS {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
		:m_Name(name)
	{
		Init();
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

		BGS_CORE_ASSERT(m_Window->Init(), "Cannot create Win32 window");

		Renderer::Init();

		return true;
	}

	void Application::ShutDown()
	{
		delete m_Timer;
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventManager manager(e);
		manager.Dispatch<WindowCloseEvent>(BGS_BIND_EVENT_FN(Application::OnWindowClose));
		manager.Dispatch<WindowResizeEvent>(BGS_BIND_EVENT_FN(Application::OnWindowResize));
		//manager.Dispatch<MouseMovedEvent>(BGS_BIND_EVENT_FN(Application::OnMouseMoved));
		//manager.Dispatch<MouseButtonPressedEvent>(BGS_BIND_EVENT_FN(Application::OnMousePressed));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		m_Timer = new Utils::Timer();
		float timer = 0.0f;
		uint32_t frames = 0;
		

		while (m_Running)
		{	
			float time = m_Timer->ElapsedMillis();
			Utils::Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate(timestep);
				frames++;
			}

			m_Window->OnUpdate();
			if (m_Timer->Elapsed() - timer > 1.0f)
			{
				timer += 1.0f;
				m_FramesPerSecond = frames;
				frames = 0;
			}
			m_Window->SetTitle(m_Name + std::string(" (FPS: ") + std::to_string(m_FramesPerSecond) + std::string(") "));
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
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;
	}
	/*
	bool Application::OnMouseMoved(MouseMovedEvent& e)
	{
		//BGS_CORE_TRACE(e.ToString());
		return true;
	}

	bool Application::OnMousePressed(MouseButtonPressedEvent& e)
	{
		//BGS_CORE_TRACE(e.ToString());
		return true;
	}
	*/
}