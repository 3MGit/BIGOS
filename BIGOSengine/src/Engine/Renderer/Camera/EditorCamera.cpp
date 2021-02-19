#include "bgspch.h"
#include "Engine/Renderer/Camera/EditorCamera.h"

#include "Engine/Core/App/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace BIGOS {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(math::mat4::Perspective(fov, aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::OnUpdate(Utils::Timestep ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const math::vec2& mouse = { Input::GetMouseX(), Input::GetMouseY() };
			math::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventManager dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BGS_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	math::vec3 EditorCamera::GetUpDirection() const
	{
		return math::quat::Rotate(GetOrientation(), math::vec3::YAxis());
	}

	math::vec3 EditorCamera::GetRightDirection() const
	{
		return math::quat::Rotate(GetOrientation(), math::vec3::XAxis());
	}

	math::vec3 EditorCamera::GetForwardDirection() const
	{
		return math::quat::Rotate(GetOrientation(), -math::vec3::ZAxis());
	}

	math::quat EditorCamera::GetOrientation() const
	{
		return math::quat::RotationY(-m_Yaw) * math::quat::RotationX(-m_Pitch);
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = math::mat4::Perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		math::quat orientation = GetOrientation();
		m_ViewMatrix = math::mat4::Translate(math::vec3(0, 0, 1))* math::mat4::Rotate(orientation.Conjugate())* math::mat4::Translate(-m_Position);
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.2f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const math::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const math::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	math::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366 * (x * x) - 0.1778 * x + 0.3021;

		float y = min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = max(distance, 0.0f);
		float speed = distance * distance;
		speed = min(speed, 100.0f); // max speed = 100
		return speed;
	}

}