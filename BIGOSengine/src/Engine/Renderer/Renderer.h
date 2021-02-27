#pragma once

#include "Engine/Renderer/API/RenderCommand.h"

namespace BIGOS {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		//TODO: High lvl functions such as Begin/EndScene(), DrawQuad() ect.

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}