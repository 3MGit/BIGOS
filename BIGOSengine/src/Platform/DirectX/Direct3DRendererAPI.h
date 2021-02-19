#pragma once
#include "Engine/Renderer/RendererAPI.h"
#include "Platform/DirectX/Direct3DContext.h"

namespace BIGOS {

	class Direct3DRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const math::vec4& color) override;
		virtual void Clear() override;

		virtual void Present() override;
		virtual void DrawIndexed(uint32_t count) override;
	private:
		Direct3DContext* m_Context;
		ID3D11RasterizerState* m_Solid;
		ID3D11RasterizerState* m_Wireframe;
		math::vec4 m_ClearColor;
	};

}