#pragma once

#include "Engine/Core/App/Window.h"

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

		virtual void SetVsync(bool enabled) override;
		virtual bool IsVSync() const override { return m_Data.Vsync; }

		virtual void SetEventCallback(const EventCallbackFn& callback) override;

		//virtual void* GetNativeWindow() const { return m_Hwnd; }
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE m_HInstance;
	};

	//void FocusCallback(Window* window, bool focused);
}