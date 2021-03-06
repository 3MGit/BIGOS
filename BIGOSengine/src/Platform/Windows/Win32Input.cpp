#include "bgspch.h"

#include "Engine/Core/App/Input.h"
#include "Engine/Core/App/Application.h"

#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"


namespace BIGOS {
	
	void InputManager::PlatformUpdate()
	{
		// Mouse Events
		POINT mouse;
		GetCursorPos(&mouse);
		auto hwnd = Application::Get().GetWindow()->GetNativeWindowHandle();
		ScreenToClient((HWND)hwnd, &mouse);

		math::vec2 mousePos = math::vec2((float)mouse.x, (float)mouse.y);
		if (mousePos != m_MousePosition)
		{
			Window* window = Window::GetWindowClass(hwnd);
			window->m_Data.EventCallback(MouseMovedEvent(mousePos.x, mousePos.y));
			//m_EventCallback(MouseMovedEvent(mousePos.x, mousePos.y));
			m_MousePosition = mousePos;
		}
	}

	void InputManager::SetMousePosition(const math::vec2& position)
	{
		POINT pt = { (LONG)position.x, (LONG)position.y };
		auto hwnd = Application::Get().GetWindow()->GetNativeWindowHandle();
		ClientToScreen((HWND)hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	void InputManager::SetMouseCursor(uint32_t cursor)
	{
		/*
		if (cursor == NULL)
		{
			SetCursor(SP_NO_CURSOR);
			while (ShowCursor(false) >= 0);
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			ShowCursor(true);
		}
		*/
	}

	void KeyCallback(InputManager* inputManager, uint32_t flags, KeyCode key, uint32_t message)
	{
		bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
		inputManager->m_KeyState[key] = pressed;

		bool repeat = (flags >> 30) & 1;

		auto hwnd = Application::Get().GetWindow()->GetNativeWindowHandle();
		Window* window = Window::GetWindowClass(hwnd);
		if (pressed)
			window->m_Data.EventCallback(KeyPressedEvent(key, repeat));
			//inputManager->m_EventCallback(KeyPressedEvent(key, repeat));
		else
			window->m_Data.EventCallback(KeyReleasedEvent(key));
			//inputManager->m_EventCallback(KeyReleasedEvent(key));
	}

	void MouseButtonCallback(InputManager* inputManager, MouseCode button, uint32_t x, uint32_t y)
	{
		auto hwnd = Application::Get().GetWindow()->GetNativeWindowHandle();
		bool down = false;
		switch (button)
		{
		case WM_LBUTTONDOWN:
			SetCapture((HWND)hwnd);
			button = Mouse::ButtonLeft;
			down = true;
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
			button = Mouse::ButtonLeft;
			down = false;
			break;
		case WM_RBUTTONDOWN:
			SetCapture((HWND)hwnd);
			button = Mouse::ButtonRight;
			down = true;
			break;
		case WM_RBUTTONUP:
			ReleaseCapture();
			button = Mouse::ButtonRight;
			down = false;
			break;
		case WM_MBUTTONDOWN:
			SetCapture((HWND)hwnd);
			button = Mouse::ButtonMiddle;
			down = true;
			break;
		case WM_MBUTTONUP:
			ReleaseCapture();
			button = Mouse::ButtonMiddle;
			down = false;
			break;
		}
		inputManager->m_MouseButtons[button] = down;
		inputManager->m_MousePosition.x = (float)x;
		inputManager->m_MousePosition.y = (float)y;

		//BGS_CORE_ASSERT(inputManager->m_EventCallback);
		Window* window = Window::GetWindowClass(hwnd);
		if (down)
			window->m_Data.EventCallback(MouseButtonPressedEvent(button));
			//inputManager->m_EventCallback(MouseButtonPressedEvent(button));
		else
			window->m_Data.EventCallback(MouseButtonReleasedEvent(button));
			//inputManager->m_EventCallback(MouseButtonReleasedEvent(button));
	}

	void MouseScrollCallback(InputManager* inputManager, uint32_t message, float xOffSet, float yOffSet) 
	{
		auto hwnd = Application::Get().GetWindow()->GetNativeWindowHandle();
		Window* window = Window::GetWindowClass((HWND)hwnd);
		window->m_Data.EventCallback(MouseScrolledEvent(0, yOffSet));
	}

}