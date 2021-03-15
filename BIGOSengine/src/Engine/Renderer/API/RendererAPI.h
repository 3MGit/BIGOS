#pragma once
#include "bgspch.h"

#include "Engine/math/vec4.h"

namespace BIGOS {

	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0, OPENGL = 1, DIRECT3D = 2, VULKAN = 3
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const math::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetBlending(bool enabled) = 0;
		virtual void SetDepthTesting(bool enabled) = 0;
		virtual void SetWireframeMode(bool enabled) = 0;

		virtual void Present() = 0;
		virtual void DrawIndexed(uint32_t count) = 0;

		static API GetAPI() { return s_API; }
		static std::unique_ptr<RendererAPI> Create();
	private:
		static API s_API;
	};
	
}