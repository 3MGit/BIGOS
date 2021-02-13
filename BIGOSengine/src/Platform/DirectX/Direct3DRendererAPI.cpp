#include "bgspch.h"
#include "Platform/DirectX/Direct3DRendererAPI.h"

namespace BIGOS {

	void Direct3DRendererAPI::Init()
	{
		// SetViewport is called earlier than Init();
		//m_Context = Direct3DContext::GetContext();

		// TODO: Log device and drivers properties
	}

	void Direct3DRendererAPI::Shutdown()
	{
		m_Context->Shutdown();
	}

	void Direct3DRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		// Read accet violitation
		m_Context = Direct3DContext::GetContext();
		m_Context->SetViewport(x, y, width, height);	
	}

	void Direct3DRendererAPI::SetClearColor(const math::vec4& color)
	{
		m_ClearColor = color;
	}

	void Direct3DRendererAPI::Clear()
	{
		float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
		Direct3DContext::GetDeviceContext()->ClearRenderTargetView(Direct3DContext::GetBackBuffer(), color);
		Direct3DContext::GetDeviceContext()->ClearDepthStencilView(Direct3DContext::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Direct3DRendererAPI::Present()
	{
		m_Context->Present();
	}

	void Direct3DRendererAPI::Draw(UINT vertex_count, UINT start_vertex_index)
	{
		Direct3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Direct3DContext::GetDeviceContext()->Draw(vertex_count, start_vertex_index);
		//Direct3DContext::GetDeviceContext()->DrawIndexed()
	}

}