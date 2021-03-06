#pragma once

#include "Engine/Renderer/API/RendererAPI.h"

namespace BIGOS {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void Shutdown()
		{
			s_RendererAPI->Shutdown();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const math::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}
		
		static void DrawIndexed(uint32_t count)
		{
			s_RendererAPI->DrawIndexed(count);
		}

		static void SetBlending(bool enabled)
		{
			s_RendererAPI->SetBlending(enabled);
		}

		static void SetDepthTesting(bool enabled) 
		{
			s_RendererAPI->SetDepthTesting(enabled);
		}

		static void SetWireframeMode(bool enabled)
		{
			s_RendererAPI->SetWireframeMode(enabled);
		}

	private:
		// Think should that be handled by RenderCommand, only window should use that metod
		static void Present()
		{
			s_RendererAPI->Present();
		}
		friend class WindowsWindow;

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}