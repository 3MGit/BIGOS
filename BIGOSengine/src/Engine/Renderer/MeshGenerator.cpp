#include "bgspch.h"
#include "Engine/Renderer/MeshGenerator.h"

namespace BIGOS {

	Mesh* MeshGenerator::CreateCube(float size)
	{
		using namespace math;

		Vertex data[8];

		memset(data, 0, 8 * sizeof(Vertex));

		data[0].Position = vec3(-size / 2.0f, -size / 2.0f, size / 2.0f);
		data[1].Position = vec3(size / 2.0f, -size / 2.0f, size / 2.0f);
		data[2].Position = vec3(size / 2.0f, size / 2.0f, size / 2.0f);
		data[3].Position = vec3(-size / 2.0f, size / 2.0f, size / 2.0f);
		data[4].Position = vec3(-size / 2.0f, -size / 2.0f, -size / 2.0f);
		data[5].Position = vec3(size / 2.0f, -size / 2.0f, -size / 2.0f);
		data[6].Position = vec3(size / 2.0f, size / 2.0f, -size / 2.0f);
		data[7].Position = vec3(-size / 2.0f, size / 2.0f, -size / 2.0f);

		data[0].Normal = vec3(-1.0f, -1.0f, 1.0f);
		data[1].Normal = vec3(1.0f, -1.0f, 1.0f);
		data[2].Normal = vec3(1.0f, 1.0f, 1.0f);
		data[3].Normal = vec3(-1.0f, 1.0f, 1.0f);
		data[4].Normal = vec3(-1.0f, -1.0f, -1.0f);
		data[5].Normal = vec3(1.0f, -1.0f, -1.0f);
		data[6].Normal = vec3(1.0f, 1.0f, -1.0f);
		data[7].Normal = vec3(-1.0f, 1.0f, -1.0f);

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(8 * sizeof(Vertex));
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	}
			});
		vb->SetData(data, 8 * sizeof(Vertex));

		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0,
			3, 2, 6, 6, 7, 3,
			7, 6, 5, 5, 4, 7,
			4, 0, 3, 3, 7, 4,
			0, 1, 5, 5, 4, 0,
			1, 5, 6, 6, 2, 1
		};

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices, ARRAYSIZE(indices));

		return new Mesh(vb, ib);
	}

}


