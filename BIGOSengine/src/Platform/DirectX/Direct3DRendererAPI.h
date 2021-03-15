#pragma once
#include "Engine/Renderer/API/RendererAPI.h"
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

		virtual void SetBlending(bool enabled) override;
		virtual void SetDepthTesting(bool enabled) override;
		virtual void SetWireframeMode(bool enabled) override;

		virtual void Present() override;
		virtual void DrawIndexed(uint32_t count) override;
	private:
		void CreateBlendStates();
		void CreateDepthStencilStates();
		void CreateRasterizerStates();
	private:
		//Direct3DContext* m_Context;
		math::vec4 m_ClearColor;

		static ID3D11RasterizerState* s_RasterizerStates[2];
		static ID3D11BlendState* s_BlendStates[2];
		static ID3D11DepthStencilState* s_DepthStencilStates[2];
	};

}