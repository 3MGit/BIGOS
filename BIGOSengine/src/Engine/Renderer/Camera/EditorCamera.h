#pragma once

#include "Engine/Utils/Timestep.h"

#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/Renderer/Camera/Camera.h"

#include "Engine/math/vec2.h"
#include "Engine/math/vec3.h"
#include "Engine/math/mat4.h"
#include "Engine/math/quaternion.h"


namespace BIGOS {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Utils::Timestep ts);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

		const math::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		math::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
	
		math::vec3 GetUpDirection() const;
		math::vec3 GetRightDirection() const;
		math::vec3 GetForwardDirection() const;
		const math::vec3& GetPosition() const { return m_Position; }
		math::quat GetOrientation() const;
		
		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const math::vec2& delta);
		void MouseRotate(const math::vec2& delta);
		void MouseZoom(float delta);

		math::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		math::mat4 m_ViewMatrix;
		math::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		math::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		math::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

		float m_Distance = 25.f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}