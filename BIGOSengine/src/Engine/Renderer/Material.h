#pragma once

#include "Engine/math/vec4.h"

namespace BIGOS {

	__declspec(align(16))
		struct Material
	{
		math::vec4 Ambient;
		math::vec4 Diffuse;
		math::vec4 Specular;

		Material()
		{

		}

		Material(math::vec4 ambient, math::vec4 diffuse, math::vec4 specular)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular) {}
	};

}