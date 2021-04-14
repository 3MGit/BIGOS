#include "bgspch.h"
#include "Platform/DX11/DX11RendererAPI.h"
#include "Platform/DX11/DX11Common.h"

namespace BIGOS {

	ID3D11RasterizerState* DX11RendererAPI::s_RasterizerStates[2];
	ID3D11BlendState* DX11RendererAPI::s_BlendStates[2];
	ID3D11DepthStencilState* DX11RendererAPI::s_DepthStencilStates[2];

	void DX11RendererAPI::Init()
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

	void DX11RendererAPI::Shutdown()
	{
		for (size_t i = 0; i < 2; i++)
		{
			ReleaseCOM(s_RasterizerStates[i]);
			ReleaseCOM(s_BlendStates[i]);
			ReleaseCOM(s_DepthStencilStates[i]);
		}
		DX11Context::GetContext()->Shutdown();
	}

	void DX11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		//m_Context = Direct3DContext::GetContext();
		DX11Context::GetContext()->SetViewport(x, y, width, height);
	}

	void DX11RendererAPI::SetClearColor(const math::vec4& color)
	{
		m_ClearColor = color;
	}

	void DX11RendererAPI::Clear()
	{
		float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
		DX11Context::GetDeviceContext()->ClearRenderTargetView(DX11Context::GetRenderTargetView(), color);
		DX11Context::GetDeviceContext()->ClearDepthStencilView(DX11Context::GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11RendererAPI::SetBlending(bool enabled)
	{
		DX11Context::GetDeviceContext()->OMSetBlendState(enabled ? s_BlendStates[1] : s_BlendStates[0], NULL, 0xffffffff);
	}

	void DX11RendererAPI::SetDepthTesting(bool enabled)
	{
		DX11Context::GetDeviceContext()->OMSetDepthStencilState(enabled ? s_DepthStencilStates[0] : s_DepthStencilStates[1], NULL);
	}

	void DX11RendererAPI::SetWireframeMode(bool enabled)
	{
		DX11Context::GetDeviceContext()->RSSetState(enabled ? s_RasterizerStates[0]: s_RasterizerStates[1]);
	}

	void DX11RendererAPI::Present()
	{
		DX11Context::GetContext()->Present();
	}

	void DX11RendererAPI::DrawIndexed(uint32_t count)
	{
		DX11Context::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Context::GetDeviceContext()->DrawIndexed(count, 0, 0);
		// TODO: During optimilization maby expand API to handle a lot of object in one buffer and draw call
	}

	void DX11RendererAPI::CreateBlendStates()
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		DX11Context::GetDevice()->CreateBlendState(&desc, &s_BlendStates[0]);
	
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

		DX11Context::GetDevice()->CreateBlendState(&desc, &s_BlendStates[1]);
	}

	void DX11RendererAPI::CreateDepthStencilStates()
	{
		
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
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

		DX11Context::GetDevice()->CreateDepthStencilState(&desc, &s_DepthStencilStates[0]);
		
		
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

		DX11Context::GetDevice()->CreateDepthStencilState(&desc, &s_DepthStencilStates[1]);
	}
	void DX11RendererAPI::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rsDesc;
		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;
		DX11Context::GetDevice()->CreateRasterizerState(&rsDesc, &s_RasterizerStates[0]);

		ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.DepthClipEnable = true;
		DX11Context::GetDevice()->CreateRasterizerState(&rsDesc, &s_RasterizerStates[1]);
	}

}