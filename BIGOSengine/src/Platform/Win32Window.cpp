#include "bgspch.h"

#include "Engine/Core/Core.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Platform/Win32Window.h"


namespace BIGOS {

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::OnUpdate()
	{
		MSG msg = { };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)>0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool WindowsWindow::Init()
	{
		BGS_CORE_TRACE("Creating Win32 window: Title: %s, Width: %d, Height: %d", m_Data.Title.c_str(), m_Data.Width, m_Data.Height);

		// Register the window class.
		WNDCLASSA winClass = { };
		winClass.cbClsExtra = NULL;
		winClass.cbWndExtra = NULL;
		winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winClass.hInstance = NULL;
		winClass.lpszClassName = "Bigos Win32 Window";
		winClass.lpszMenuName = "";
		winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		winClass.lpfnWndProc = WindowsWindow::WindowProc;

		if (!RegisterClassA(&winClass))
		{
			BGS_CORE_FATAL("Could not register Win32 class!");
			return false;
		}

		RECT size = { 0, 0, (LONG)m_Data.Width, (LONG)m_Data.Height };
		AdjustWindowRectEx(&size, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);


		m_Hwnd = ::CreateWindowExA(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,                             // Optional window styles.
			winClass.lpszClassName,         // Window class
			(LPCSTR)m_Data.Title.c_str(),		    // Window text
			WS_OVERLAPPEDWINDOW,            // Window style

			// Size and position
			GetSystemMetrics(SM_CXSCREEN) / 2 - m_Data.Width / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - m_Data.Height / 2,
			size.right + (-size.left), 
			size.bottom + (-size.top),

			NULL,       // Parent window    
			NULL,       // Menu
			NULL,  // Instance handle
			NULL        // Additional application data
		);

		if (!m_Hwnd)
		{
			BGS_CORE_FATAL("Could not create Win32 window!");
			return false;
		}

		RegisterWindowClass(m_Hwnd, this);

		BGS_CORE_TRACE("Win32 window succesfully created!");
		//show up the window
		ShowWindow(m_Hwnd, SW_SHOW);
		UpdateWindow(m_Hwnd);

		return true;
	}

	void WindowsWindow::ShutDown()
	{
		//Destroy window
		if (m_Hwnd)
			if(DestroyWindow(m_Hwnd))
				BGS_CORE_TRACE("Win32 window succesfully destroyed!");

	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = Window::GetWindowClass(hwnd);
		if (window == nullptr)
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		
		switch (msg)
		{
		case WM_CREATE:
		{
			// Event fired when the window is created
			
			break;
		}
		case WM_DESTROY:
		{
			// Event fierd when the window is destroyed
			WindowCloseEvent event;
			//BGS_CORE_INFO(event.ToString());
			window->m_Data.EventCallback(event);
			PostQuitMessage(0);

			break;
		}
		default:
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		}	

		return NULL;
	}

}