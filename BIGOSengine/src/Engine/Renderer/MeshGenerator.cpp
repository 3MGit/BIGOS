#include "bgspch.h"
#include "Engine/Renderer/MeshGenerator.h"
#include "Engine/math/math_func.h"

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

	Mesh* MeshGenerator::CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount)
	{
		using namespace math;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Vertex topVertex = { { 0.0f, +radius, 0.0f }, { 0.0f, +1.0f, 0.0f } };
		Vertex bottomVertex = { { 0.0f, -radius, 0.0f }, { 0.0f, -1.0f, 0.0f } };

		vertices.push_back(topVertex);

		float phiStep = PI / stackCount;
		float thetaStep = 2.0f * PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.Position.x = radius * sin(phi) * cos(theta);
				v.Position.y = radius * cos(phi);
				v.Position.z = radius * sin(phi) * sin(theta);

				//v.Normal = v.Position.Normalize();
				v.Normal.x = 1.0f;
				v.Normal.y = 1.0f;
				v.Normal.z = 1.0f;

				vertices.push_back(v);
			}
		}

		vertices.push_back(bottomVertex);

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices.size());
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	}
			});
		vb->SetData(vertices.data(), vertices.size());

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				indices.push_back(baseIndex + i * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			indices.push_back(southPoleIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);
		}

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices.data(),indices.size());

		return new Mesh(vb, ib);
	}

}


