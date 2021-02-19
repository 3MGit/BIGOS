#pragma once

#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

#include "Engine/Events/Event.h"

#include "Engine/math/vec2.h"

namespace BIGOS {

#define MAX_KEYS	1024
#define MAX_BUTTONS	32

	using EventCallbackFn = std::function<void(Event&)>;

	class Window;

	class InputManager
	{
	private:
		friend class Window;
	private:
		bool m_KeyState[MAX_KEYS];
		bool m_LastKeyState[MAX_KEYS];

		bool m_MouseButtons[MAX_BUTTONS];
		bool m_MouseState[MAX_BUTTONS];
		bool m_MouseClicked[MAX_BUTTONS];
		bool m_MouseGrabbed;

		math::vec2 m_MousePosition;
		//EventCallbackFn m_EventCallback;
	public:
		InputManager();

		//void SetEventCallback(const EventCallbackFn& eventCallback) { m_EventCallback = eventCallback; }

		void Update();
		void PlatformUpdate();

		bool IsKeyPressed(KeyCode key) const;
		bool IsMouseButtonPressed(MouseCode button) const;
		bool IsMouseButtonClicked(MouseCode button) const;

		const math::vec2& GetMousePosition() const;
		void SetMousePosition(const math::vec2& position);
		const bool IsMouseGrabbed() const;
		void SetMouseGrabbed(bool grabbed);
		void SetMouseCursor(uint32_t cursor);

		void ClearKeys();
		void ClearMouseButtons();
	private:
		friend void KeyCallback(InputManager* inputManager, uint32_t flags, KeyCode key, uint32_t message);
		friend void MouseButtonCallback(InputManager* inputManager, MouseCode button, uint32_t x, uint32_t y);
		friend void MouseScrollCallback(InputManager* inputManager, uint32_t message, float xOffSet, float yOffSet);
	};

	class Input
	{
	private:
		friend class InputManager;
	private:
		static InputManager* s_InputManager;
	public:
		static bool IsKeyPressed(KeyCode key) { return s_InputManager->IsKeyPressed(key); }

		static bool IsMouseButtonPressed(MouseCode button) { return s_InputManager->IsMouseButtonPressed(button); }
		static math::vec2 GetMousePosition() { return s_InputManager->GetMousePosition(); }
		static float GetMouseX() { return s_InputManager->GetMousePosition().x; }
		static float GetMouseY() { return s_InputManager->GetMousePosition().y; }

		inline static InputManager* GetInputManager() { return s_InputManager; }
	};

}