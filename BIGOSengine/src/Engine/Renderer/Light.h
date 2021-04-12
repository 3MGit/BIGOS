#pragma once

#include "Engine/math/vec4.h"
#include "Engine/math/vec3.h"

namespace BIGOS {


	__declspec(align(16)) 
	struct Light
	{
		math::vec4 Color;
		math::vec3 Position;
		math::vec3 Direction;
		float Intensity;

		Light()
		{

		}

		Light(const math::vec3& direction, const math::vec3& position, float intensity = 10.0f, const math::vec4& color = math::vec4(300.0f))
			: Direction(direction), Position(position), Color(color), Intensity(intensity) {}
	};

	__declspec(align(16)) 
	struct PhongLight
	{
		math::vec4 Ambient;
		math::vec4 Diffuse;
		math::vec4 Specular;

		math::vec3 Direction;

		PhongLight()
		{

		}

		PhongLight(const math::vec4& ambient, const math::vec4& diffuse, const math::vec4& specular, const math::vec3& direction)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Direction(direction) {}
	};

}