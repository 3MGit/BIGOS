#pragma once

#include "Engine/math/vec4.h"
#include "Engine/math/vec3.h"

namespace BIGOS {

	__declspec(align(16)) 
	struct Light
	{
		BIGOS::math::vec4 Ambient;
		BIGOS::math::vec4 Diffuse;
		BIGOS::math::vec4 Specular;
		BIGOS::math::vec3 Direction;


	};

}