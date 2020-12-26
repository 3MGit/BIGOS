#include "bgspch.h"
#include "Engine/Core/App/Input.h"

namespace BIGOS {

	InputManager* Input::s_InputManager = nullptr;

	InputManager::InputManager()
	{
		ClearKeys();
		ClearMouseButtons();

		m_MouseGrabbed = true;

		Input::s_InputManager = this;

		// m_KeyState = spnew bool[MAX_KEYS];
		// m_LastKeyState = spnew bool[MAX_KEYS];
	}

	void InputManager::Update()
	{
		for (int32_t i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];

		memcpy(m_LastKeyState, m_KeyState, MAX_KEYS);
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	void InputManager::ClearKeys()
	{
		for (int32_t i = 0; i < MAX_KEYS; i++)
		{
			m_KeyState[i] = false;
			m_LastKeyState[i] = false;
		}
	}

	void InputManager::ClearMouseButtons()
	{
		for (int32_t i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseState[i] = false;
			m_MouseClicked[i] = false;
		}
	}

	bool InputManager::IsKeyPressed(KeyCode key) const
	{
		// TODO: Log this!
		if (key >= MAX_KEYS)
			return false;

		return m_KeyState[key];
	}

	bool InputManager::IsMouseButtonPressed(MouseCode button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseButtons[button];
	}

	bool InputManager::IsMouseButtonClicked(MouseCode button) const
	{
		// TODO: Log this!
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseClicked[button];
	}

	const math::vec2& InputManager::GetMousePosition() const
	{
		return m_MousePosition;
	}

	const bool InputManager::IsMouseGrabbed() const
	{
		return m_MouseGrabbed;
	}

	void InputManager::SetMouseGrabbed(bool grabbed)
	{
		m_MouseGrabbed = grabbed;
	}

}