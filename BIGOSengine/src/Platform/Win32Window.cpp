#include "bgspch.h"

#include "Engine/Core/Core.h"
#include "Engine/Events/ApplicationEvent.h"

#include "Platform/Win32Window.h"


namespace BIGOS {

	HWND m_Hwnd;

	extern void MouseButtonCallback(InputManager* inputManager, MouseCode button, uint32_t x, uint32_t y);
	extern void KeyCallback(InputManager* inputManager, uint32_t flags, KeyCode key, uint32_t message);

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

		m_Data.m_InputManager->PlatformUpdate();
	}

	bool WindowsWindow::Init()
	{
		BGS_CORE_TRACE("Creating Win32 window: Title: %s, Width: %d, Height: %d", m_Data.Title.c_str(), m_Data.Width, m_Data.Height);

		m_Data.m_InputManager = new InputManager();

		// Register the window class.
		WNDCLASSA winClass = { };
		winClass.cbClsExtra = NULL;
		winClass.cbWndExtra = NULL;
		winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		winClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		winClass.hInstance = m_HInstance;
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
			m_HInstance,  // Instance handle
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

	void WindowsWindow::SetVsync(bool enabled)
	{
		m_Data.m_Vsync = enabled;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
		//m_Data.m_InputManager->SetEventCallback(m_Data.EventCallback);
	};

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = Window::GetWindowClass(hwnd);
		if (window == nullptr)
			return DefWindowProcA(hwnd, msg, wParam, lParam);

		InputManager* inputManager = window->m_Data.m_InputManager;
		
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
			window->m_Data.EventCallback(event);
			PostQuitMessage(0);

			break;
		}
		case WM_SIZE:
		{
			// Event fierd when the window is resized
			window->m_Data.Width = LOWORD(lParam);
			window->m_Data.Height = HIWORD(lParam);

			WindowResizeEvent event(window->m_Data.Width, window->m_Data.Height);
			window->m_Data.EventCallback(event);
			break;
		}
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			KeyCallback(inputManager, lParam, wParam, msg);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			MouseButtonCallback(inputManager, msg, LOWORD(lParam), HIWORD(lParam));
			break;
		default:
			return DefWindowProcA(hwnd, msg, wParam, lParam);
		}	

		return NULL;
	}

}