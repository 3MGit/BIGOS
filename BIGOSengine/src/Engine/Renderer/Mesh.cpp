#include "bgspch.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/API/RenderCommand.h"


namespace BIGOS {

	Mesh::Mesh(std::shared_ptr<BIGOS::VertexBuffer> vb, std::shared_ptr<BIGOS::IndexBuffer> ib)
		:m_IndexBuffer(ib), m_VertexBuffer(vb)
	{
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::Render()
	{
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		RenderCommand::DrawIndexed(m_IndexBuffer->GetCount());
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
	}

}