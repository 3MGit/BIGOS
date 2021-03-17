#include "bgspch.h"
#include "Engine/Renderer/MeshGenerator.h"
#include "Engine/math/math_func.h"

namespace BIGOS {

	Mesh* MeshGenerator::CreateBox(math::vec3 size)
	{
		using namespace math;

		Vertex data[24];
		uint32_t indices[36];

		float w2 = 0.5f * size.x;
		float h2 = 0.5f * size.y;
		float d2 = 0.5f * size.z;

		// Fill in the front face vertex data.
		data[0].Position = vec3(-w2, -h2, -d2);		data[0].Normal = vec3(0.0f, 0.0f, -1.0f); data[0].TangentU = vec3(1.0f, 0.0f, 0.0f);	data[0].UV = vec2(0.0f, 1.0f);
		data[1].Position = vec3(-w2, +h2, -d2);		data[1].Normal = vec3(0.0f, 0.0f, -1.0f); data[1].TangentU = vec3(1.0f, 0.0f, 0.0f);	data[1].UV = vec2(0.0f, 0.0f);
		data[2].Position = vec3(+w2, +h2, -d2);		data[2].Normal = vec3(0.0f, 0.0f, -1.0f); data[2].TangentU = vec3(1.0f, 0.0f, 0.0f);	data[2].UV = vec2(1.0f, 0.0f);
		data[3].Position = vec3(+w2, -h2, -d2);		data[3].Normal = vec3(0.0f, 0.0f, -1.0f); data[3].TangentU = vec3(1.0f, 0.0f, 0.0f);	data[3].UV = vec2(1.0f, 1.0f);

		// Fill in the back face vertex data.
		data[4].Position = vec3(-w2, -h2, +d2);		data[4].Normal = vec3(0.0f, 0.0f, 1.0f);	data[4].TangentU = vec3(-1.0f, 0.0f, 0.0f);	 data[4].UV = vec2(1.0f, 1.0f);
		data[5].Position = vec3(+w2, -h2, +d2);		data[5].Normal = vec3(0.0f, 0.0f, 1.0f);	data[5].TangentU = vec3(-1.0f, 0.0f, 0.0f);	 data[5].UV = vec2(0.0f, 1.0f);
		data[6].Position = vec3(+w2, +h2, +d2);		data[6].Normal = vec3(0.0f, 0.0f, 1.0f);	data[6].TangentU = vec3(-1.0f, 0.0f, 0.0f);	 data[6].UV = vec2(0.0f, 0.0f);
		data[7].Position = vec3(-w2, +h2, +d2);		data[7].Normal = vec3(0.0f, 0.0f, 1.0f);	data[7].TangentU = vec3(-1.0f, 0.0f, 0.0f);	 data[7].UV = vec2(1.0f, 0.0f);

		// Fill in the top face vertex data.
		data[8].Position = vec3(-w2, +h2, -d2);		data[8].Normal = vec3(0.0f, 1.0f, 0.0f);	data[8].TangentU = vec3(1.0f, 0.0f, 0.0f); data[8].UV = vec2(0.0f, 1.0f);
		data[9].Position = vec3(-w2, +h2, +d2);		data[9].Normal = vec3(0.0f, 1.0f, 0.0f);	data[9].TangentU = vec3(1.0f, 0.0f, 0.0f); data[9].UV = vec2(0.0f, 0.0f);
		data[10].Position = vec3(+w2, +h2, +d2);	data[10].Normal = vec3(0.0f, 1.0f, 0.0f);	data[10].TangentU = vec3(1.0f, 0.0f, 0.0f); data[10].UV = vec2(1.0f, 0.0f);
		data[11].Position = vec3(+w2, +h2, -d2);	data[11].Normal = vec3(0.0f, 1.0f, 0.0f);	data[11].TangentU = vec3(1.0f, 0.0f, 0.0f); data[11].UV = vec2(1.0f, 1.0f);

		// Fill in the bottom face vertex data.
		data[12].Position = vec3(-w2, -h2, -d2);	data[12].Normal = vec3(0.0f, -1.0f, 0.0f); data[12].TangentU = vec3(-1.0f, 0.0f, 0.0f); 	data[12].UV = vec2(1.0f, 1.0f);
		data[13].Position = vec3(+w2, -h2, -d2);	data[13].Normal = vec3(0.0f, -1.0f, 0.0f); data[13].TangentU = vec3(-1.0f, 0.0f, 0.0f); 	data[13].UV = vec2(0.0f, 1.0f);
		data[14].Position = vec3(+w2, -h2, +d2);	data[14].Normal = vec3(0.0f, -1.0f, 0.0f); data[14].TangentU = vec3(-1.0f, 0.0f, 0.0f);		data[14].UV = vec2(0.0f, 0.0f);
		data[15].Position = vec3(-w2, -h2, +d2);	data[15].Normal = vec3(0.0f, -1.0f, 0.0f); data[15].TangentU = vec3(-1.0f, 0.0f, 0.0f);		data[15].UV = vec2(1.0f, 0.0f);

		// Fill in the left face vertex data.
		data[16].Position = vec3(-w2, -h2, +d2);	data[16].Normal = vec3(-1.0f, 0.0f, 0.0f); data[16].TangentU = vec3(0.0f, 0.0f, -1.0f);	data[16].UV = vec2(0.0f, 1.0f);
		data[17].Position = vec3(-w2, +h2, +d2);	data[17].Normal = vec3(-1.0f, 0.0f, 0.0f); data[17].TangentU = vec3(0.0f, 0.0f, -1.0f);	data[17].UV = vec2(0.0f, 0.0f);
		data[18].Position = vec3(-w2, +h2, -d2);	data[18].Normal = vec3(-1.0f, 0.0f, 0.0f); data[18].TangentU = vec3(0.0f, 0.0f, -1.0f);	data[18].UV = vec2(1.0f, 0.0f);
		data[19].Position = vec3(-w2, -h2, -d2);	data[19].Normal = vec3(-1.0f, 0.0f, 0.0f); data[19].TangentU = vec3(0.0f, 0.0f, -1.0f);	data[19].UV = vec2(1.0f, 1.0f);

		// Fill in the right face vertex data.
		data[20].Position = vec3(+w2, -h2, -d2);	data[20].Normal = vec3(1.0f, 0.0f, 0.0f); data[20].TangentU = vec3(0.0f, 0.0f, 1.0f);	data[20].UV = vec2(0.0f, 1.0f);
		data[21].Position = vec3(+w2, +h2, -d2);	data[21].Normal = vec3(1.0f, 0.0f, 0.0f); data[21].TangentU = vec3(0.0f, 0.0f, 1.0f);	data[21].UV = vec2(0.0f, 0.0f);
		data[22].Position = vec3(+w2, +h2, +d2);	data[22].Normal = vec3(1.0f, 0.0f, 0.0f); data[22].TangentU = vec3(0.0f, 0.0f, 1.0f);	data[22].UV = vec2(1.0f, 0.0f);
		data[23].Position = vec3(+w2, -h2, +d2);	data[23].Normal = vec3(1.0f, 0.0f, 0.0f); data[23].TangentU = vec3(0.0f, 0.0f, 1.0f);	data[23].UV = vec2(1.0f, 1.0f);

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(24 * sizeof(Vertex));
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	},
			{ BIGOS::ShaderDataType::Float3, "TANGENT"	},
			{ BIGOS::ShaderDataType::Float2, "TEXCOORD"	}
			});
		vb->SetData(data, 24 * sizeof(Vertex));

		// Fill in the front face index data
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;

		// Fill in the back face index data
		indices[6] = 4; indices[7] = 5; indices[8] = 6;
		indices[9] = 4; indices[10] = 6; indices[11] = 7;

		// Fill in the top face index data
		indices[12] = 8; indices[13] = 9; indices[14] = 10;
		indices[15] = 8; indices[16] = 10; indices[17] = 11;

		// Fill in the bottom face index data
		indices[18] = 12; indices[19] = 13; indices[20] = 14;
		indices[21] = 12; indices[22] = 14; indices[23] = 15;

		// Fill in the left face index data
		indices[24] = 16; indices[25] = 17; indices[26] = 18;
		indices[27] = 16; indices[28] = 18; indices[29] = 19;

		// Fill in the right face index data
		indices[30] = 20; indices[31] = 21; indices[32] = 22;
		indices[33] = 20; indices[34] = 22; indices[35] = 23;

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices, ARRAYSIZE(indices));

		return new Mesh(vb, ib);
	}

	Mesh* MeshGenerator::CreateSphere(float radius, uint32_t sliceCount, uint32_t stackCount)
	{
		using namespace math;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.clear();
		indices.clear();

		Vertex topVertex = { { 0.0f, +radius, 0.0f }, { 0.0f, +1.0f, 0.0f } };
		Vertex bottomVertex = { { 0.0f, -radius, 0.0f,  }, { 0.0f, -1.0f, 0.0f } };

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

				v.Normal = v.Position.Normalize();
				//v.Normal.x = 1.0f;
				//v.Normal.y = 1.0f;
				//v.Normal.z = 1.0f;

				v.TangentU.x = -radius * sinf(phi) * sinf(theta);
				v.TangentU.y = 0.0f;
				v.TangentU.z = +radius * sinf(phi) * cosf(theta);

				v.TangentU = v.TangentU.Normalize();

				v.UV.x = theta / PI;
				v.UV.y = phi /PI;

				vertices.push_back(v);
			}
		}

		vertices.push_back(bottomVertex);

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	},
			{ BIGOS::ShaderDataType::Float3, "TANGENT"	},
			{ BIGOS::ShaderDataType::Float2, "TEXCOORD"	}
			});
		vb->SetData(vertices.data(), vertices.size() * sizeof(Vertex));

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

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());

		return new Mesh(vb, ib);
	}

	Mesh* MeshGenerator::CreateGrid(float width, float depth, uint32_t m, uint32_t n)
	{
		using namespace math;

		uint32_t vertexCount = m * n;
		uint32_t faceCount = (m - 1) * (n - 1) * 2;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		vertices.resize(vertexCount);
		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				vertices[i * n + j].Position = vec3(x, 0.0f, z);
				vertices[i * n + j].Normal = vec3(0.0f, 1.0f, 0.0f);
				vertices[i * n + j].TangentU = vec3(1.0f, 0.0f, 0.0f);

				// Stretch texture over grid.
				vertices[i * n + j].UV.x = j * du;
				vertices[i * n + j].UV.y = i * dv;
			}
		}

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	},
			{ BIGOS::ShaderDataType::Float3, "TANGENT" },
			{ BIGOS::ShaderDataType::Float2, "TEXCOORD"	}
			});
		vb->SetData(vertices.data(), vertices.size() * sizeof(Vertex));

		//
		// Create the indices.
		//

		indices.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				indices[k] = i * n + j;
				indices[k + 1] = i * n + j + 1;
				indices[k + 2] = (i + 1) * n + j;
				
				indices[k + 3] = (i + 1) * n + j;
				indices[k + 4] = i * n + j + 1;
				indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());

		return new Mesh(vb, ib);
	}

	Mesh* MeshGenerator::CreateScreen()
	{
		using namespace math;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices.resize(4);
		indices.resize(6);

		// Position coordinates specified in NDC space.
		vertices[0].Position = vec3(-1.0f, -1.0f, 0.0f);  vertices[0].Normal = vec3(0.0f, 0.0f, -1.0f); vertices[0].TangentU = vec3(1.0f, 0.0f, 0.0f); vertices[0].UV = vec2(0.0f, 1.0f);
		vertices[1].Position = vec3(-1.0f, +1.0f, 0.0f);  vertices[1].Normal = vec3(0.0f, 0.0f, -1.0f); vertices[1].TangentU = vec3(1.0f, 0.0f, 0.0f); vertices[1].UV = vec2(0.0f, 0.0f);
		vertices[2].Position = vec3(+1.0f, +1.0f, 0.0f);  vertices[2].Normal = vec3(0.0f, 0.0f, -1.0f); vertices[2].TangentU = vec3(1.0f, 0.0f, 0.0f); vertices[2].UV = vec2(1.0f, 0.0f);
		vertices[3].Position = vec3(+1.0f, -1.0f, 0.0f);  vertices[3].Normal = vec3(0.0f, 0.0f, -1.0f); vertices[3].TangentU = vec3(1.0f, 0.0f, 0.0f); vertices[3].UV = vec2(1.0f, 1.0f);

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(vertices.size() * sizeof(Vertex));
		vb->SetLayout({
			{ BIGOS::ShaderDataType::Float3, "POSITION" },
			{ BIGOS::ShaderDataType::Float3, "NORMAL"	},
			{ BIGOS::ShaderDataType::Float3, "TANGENT"	},
			{ BIGOS::ShaderDataType::Float2, "TEXCOORD"	}
			});
		vb->SetData(vertices.data(), vertices.size() * sizeof(Vertex));

		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(indices.data(), indices.size());

		return new Mesh(vb, ib);
	}

}


