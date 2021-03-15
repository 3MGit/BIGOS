#include "bgspch.h"
#include "Platform/DirectX/Direct3DRendererAPI.h"
#include "Platform/DirectX/Direct3DCommon.h"

namespace BIGOS {

	ID3D11RasterizerState* Direct3DRendererAPI::s_RasterizerStates[2];
	ID3D11BlendState* Direct3DRendererAPI::s_BlendStates[2];
	ID3D11DepthStencilState* Direct3DRendererAPI::s_DepthStencilStates[2];

	void Direct3DRendererAPI::Init()
	{
		// SetViewport is called earlier than Init();
		//m_Context = Direct3DContext::GetContext();

		CreateRasterizerStates();
		CreateDepthStencilStates();
		CreateBlendStates();

		SetWireframeMode(false);
		SetDepthTesting(true); // Stenciling dissable for now, idk how to use that
		SetBlending(true);

		// TODO: Log device and drivers properties

	}

	void Direct3DRendererAPI::Shutdown()
	{
		for (size_t i = 0; i < 2; i++)
		{
			ReleaseCOM(s_RasterizerStates[i]);
			ReleaseCOM(s_BlendStates[i]);
			ReleaseCOM(s_DepthStencilStates[i]);
		}
		Direct3DContext::GetContext()->Shutdown();
	}

	void Direct3DRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		//m_Context = Direct3DContext::GetContext();
		Direct3DContext::GetContext()->SetViewport(x, y, width, height);
	}

	void Direct3DRendererAPI::SetClearColor(const math::vec4& color)
	{
		m_ClearColor = color;
	}

	void Direct3DRendererAPI::Clear()
	{
		float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
		Direct3DContext::GetDeviceContext()->ClearRenderTargetView(Direct3DContext::GetRenderTargetView(), color);
		Direct3DContext::GetDeviceContext()->ClearDepthStencilView(Direct3DContext::GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Direct3DRendererAPI::SetBlending(bool enabled)
	{
		Direct3DContext::GetDeviceContext()->OMSetBlendState(enabled ? s_BlendStates[1] : s_BlendStates[0], NULL, 0xffffffff);
	}

	void Direct3DRendererAPI::SetDepthTesting(bool enabled)
	{
		Direct3DContext::GetDeviceContext()->OMSetDepthStencilState(enabled ? s_DepthStencilStates[0] : s_DepthStencilStates[1], NULL);
	}

	void Direct3DRendererAPI::SetWireframeMode(bool enabled)
	{
		Direct3DContext::GetDeviceContext()->RSSetState(enabled ? s_RasterizerStates[0]: s_RasterizerStates[1]);
	}

	void Direct3DRendererAPI::Present()
	{
		Direct3DContext::GetContext()->Present();
	}

	void Direct3DRendererAPI::DrawIndexed(uint32_t count)
	{
		Direct3DContext::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Direct3DContext::GetDeviceContext()->DrawIndexed(count, 0, 0);
		// TODO: During optimilization maby expand API to handle a lot of object in one buffer and draw call
	}

	void Direct3DRendererAPI::CreateBlendStates()
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Direct3DContext::GetDevice()->CreateBlendState(&desc, &s_BlendStates[0]);
	
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Direct3DContext::GetDevice()->CreateBlendState(&desc, &s_BlendStates[1]);
	}

	void Direct3DRendererAPI::CreateDepthStencilStates()
	{
		
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.StencilEnable = false;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = 0xff;

		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		Direct3DContext::GetDevice()->CreateDepthStencilState(&desc, &s_DepthStencilStates[0]);
		
		
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.StencilReadMask = 0xff;
		desc.StencilWriteMask = 0xff;

		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		Direct3DContext::GetDevice()->CreateDepthStencilState(&desc, &s_DepthStencilStates[1]);	
	}
	void Direct3DRendererAPI::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;
		Direct3DContext::GetDevice()->CreateRasterizerState(&rsDesc, &s_RasterizerStates[0]);

		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;
		Direct3DContext::GetDevice()->CreateRasterizerState(&rsDesc, &s_RasterizerStates[1]);
	}

}