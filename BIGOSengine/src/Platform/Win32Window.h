#pragma once

#include "Engine/Core/Window.h"

namespace BIGOS {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow() = default;
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();
		
		virtual bool Init() override;
		virtual void ShutDown() override;

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override { return m_Data.Width; };
		virtual uint32_t GetHeight() const override { return m_Data.Height; };

		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND m_Hwnd;
		HINSTANCE m_HInstance;
	};

}