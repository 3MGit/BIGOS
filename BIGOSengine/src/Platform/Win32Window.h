#pragma once

#include "Engine/Core/Window.h"

namespace BIGOS {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();
		
		virtual bool Init() override;
		virtual void ShutDown() override;

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; };
		virtual uint32_t GetHeight() const override { return m_Data.Height; };
	private:
		HWND m_Hwnd;
		HINSTANCE m_HInstance;

		struct WindowData {
			std::string Title;
			uint32_t Width, Height;
		};

		WindowData m_Data;
	};

}