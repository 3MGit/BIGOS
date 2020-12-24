#pragma once

namespace BIGOS {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	// TODO: think about defining it
	Application* CreateApplication();
}