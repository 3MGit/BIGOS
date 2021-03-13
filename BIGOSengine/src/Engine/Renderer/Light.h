#pragma once

#include "Engine/math/vec4.h"
#include "Engine/math/vec3.h"

namespace BIGOS {

	__declspec(align(16)) 
	struct Light
	{
		math::vec4 Ambient;
		math::vec4 Diffuse;
		math::vec4 Specular;

		math::vec3 Direction;

		Light()
		{

		}

		Light(math::vec4 ambient, math::vec4 diffuse, math::vec4 specular, math::vec3 direction)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular), Direction(direction) {}
	};

}