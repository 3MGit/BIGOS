#pragma once

#include "Engine/Utils/Timer.h"
#include "Engine/Utils/Timestep.h"

#include "Engine/Core/App/Window.h"
#include "Engine/Core/App/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"

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

		void Close();

		Window* GetWindow() const { return m_Window; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline uint32_t GetUPS() const { return m_UpdatesPerSecond; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Window* m_Window = nullptr;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		std::string m_Name;
		Utils::Timer* m_Timer = nullptr;
		float m_LastFrameTime = 0.0f;
		LayerStack m_LayerStack;

		uint32_t m_UpdatesPerSecond;
	private:
		friend int ::main(int argc, char** argv);
		static Application* s_Instance;
	};

	// TODO: think about defining it
	Application* CreateApplication();
}