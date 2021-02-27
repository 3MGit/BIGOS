#pragma once

#include "Engine/math/vec3.h"
#include "Engine/Renderer/Mesh.h"

namespace BIGOS {

	struct Vertex
	{
		math::vec3 Position;
		math::vec3 Normal;
	};

	namespace MeshGenerator
	{
		Mesh* CreateCube(float size);
	}	
}