#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace BIGOS {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
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

		static void Present()
		{
			s_RendererAPI->Present();
		}

		//TODO: void Draw();
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}