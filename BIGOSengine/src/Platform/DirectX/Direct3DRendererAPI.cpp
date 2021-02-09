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
		// Naruszenie zasad dostêpu, na razie chyba nie potrzbne
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
	}

	void Direct3DRendererAPI::Present()
	{
		m_Context->Present();
	}

}