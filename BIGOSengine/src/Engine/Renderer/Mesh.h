#pragma once

#include "Engine/Renderer/API/Buffer.h"

namespace BIGOS {

	class Mesh
	{
	public:
		Mesh(std::shared_ptr<BIGOS::VertexBuffer> vb, std::shared_ptr<BIGOS::IndexBuffer> ib);
		~Mesh();

		void Render();
	private:
		std::shared_ptr<BIGOS::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<BIGOS::IndexBuffer> m_IndexBuffer;
	};

}