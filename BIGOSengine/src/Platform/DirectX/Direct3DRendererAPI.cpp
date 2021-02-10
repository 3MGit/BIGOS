#include "bgspch.h"
#include "Platform/DirectX/Direct3DRendererAPI.h"

namespace BIGOS {

	void Direct3DRendererAPI::Init()
	{
		m_Context = Direct3DContext::GetContext();
	}

	void Direct3DRendererAPI::Shutdown()
	{
	}

	void Direct3DRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// Naruszenie zasad dostêpu, na razie chyba nie potrzbne, DZIA£A BEZ TEGO
		//m_Context->SetViewport(x, y, width, height);
	}

	void Direct3DRendererAPI::SetClearColor(const math::vec4& color)
	{
		m_ClearColor = color;
	}

	void Direct3DRendererAPI::Clear()
	{
		float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
		m_Context->GetDeviceContext()->ClearRenderTargetView(m_Context->GetBackBuffer(), color);
		m_Context->GetDeviceContext()->OMSetRenderTargets(1, &m_Context->m_RenderTargetView, NULL); //need to be ptr
	}

	void Direct3DRendererAPI::Present()
	{
		m_Context->Present();
	}

	void Direct3DRendererAPI::Draw(UINT vertex_count, UINT start_vertex_index)
	{
		m_Context->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_Context->GetDeviceContext()->Draw(vertex_count, start_vertex_index);
	}

}