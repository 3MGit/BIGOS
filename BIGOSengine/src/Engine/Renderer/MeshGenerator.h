#pragma once

#include "Engine/math/vec3.h"
#include "Engine/math/vec2.h"
#include "Engine/Renderer/Mesh.h"

namespace BIGOS {

	struct Vertex
	{
		math::vec3 Position;
		math::vec3 Normal;
		math::vec2 UV;
	};

	namespace MeshGenerator
	{
		Mesh* CreateSmoothCube(float size);
		Mesh* CreateBox(math::vec3 size);
		Mesh* CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount);
		Mesh* CreateGrid(float width, float depth, uint32_t m, uint32_t n);
	}	
}