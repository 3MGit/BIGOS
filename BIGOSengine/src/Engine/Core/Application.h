#pragma once

#include "Engine/Core/Window.h"

int main(int argc, char** argv);

namespace BIGOS {

	class Application
	{
	public:
		Application(const std::string& name);
		virtual ~Application();

		bool Init();
		void ShutDown();

	private:
		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		std::string m_Name;
	private:
		friend int ::main(int argc, char** argv);
	};

	// TODO: think about defining it
	Application* CreateApplication();
}