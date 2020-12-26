#pragma once

#include "Engine/Core/App/Window.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

int main(int argc, char** argv);

namespace BIGOS {

	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		bool Init();
		void ShutDown();

		void OnEvent(Event& e);

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
	private:
		Window* m_Window = nullptr;
		bool m_Running = true;
		std::string m_Name;
	private:
		friend int ::main(int argc, char** argv);
		static Application* s_Instance;
	};

	// TODO: think about defining it
	Application* CreateApplication();
}