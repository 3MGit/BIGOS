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

		Light()
		{

		}

		Light(math::vec4 color, math::vec3 position, math::vec3 direction)
			: Color(color), Position(position), Direction(direction) {}
	};

}