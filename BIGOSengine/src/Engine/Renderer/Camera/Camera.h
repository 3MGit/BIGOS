#pragma once
#include "Engine/math/mat4.h"

namespace BIGOS {

	class Camera
	{
	public: 
		Camera() = default;
		Camera(const math::mat4& projection) 
			: m_Projection(projection)
		{}

		virtual ~Camera() = default;

		const math::mat4& GetProjection() const { return m_Projection; }
	protected:
		math::mat4 m_Projection = math::mat4(1.0f);
	};

}