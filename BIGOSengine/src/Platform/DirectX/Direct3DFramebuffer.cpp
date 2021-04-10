#include "bgspch.h"
#include "Platform/DirectX/Direct3DFramebuffer.h"
#include "Engine/Core/Core.h"
#include "Platform/DirectX/Direct3DCommon.h"
#include "Platform/DirectX/Direct3DContext.h"

namespace BIGOS {

	static DXGI_FORMAT BIGOSTextureFormatToDXGIFormat(FramebufferTextureFormat format)
	{
		DXGI_FORMAT dxgiFormat;

		switch (format)
		{
		case FramebufferTextureFormat::RGBA8:
			dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			dxgiFormat = DXGI_FORMAT_UNKNOWN;
		}
		BGS_CORE_ASSERT(dxgiFormat, "Framebuffer texture format is not supported");

		return dxgiFormat;
	}

	Direct3DFramebuffer::Direct3DFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		m_RenderTargetTexture = nullptr;
		m_RenderTargetView = nullptr;
		m_ShaderResourceView = nullptr;

		Invalidate();
	}

	Direct3DFramebuffer::~Direct3DFramebuffer()
	{
		ReleaseCOM(m_RenderTargetTexture);
		BGS_CORE_TRACE("FRAMEBUFFER::TEXTURE succesfully released!");
		ReleaseCOM(m_RenderTargetView);
		BGS_CORE_TRACE("FRAMEBUFFER::RENDERTARGETVIEW succesfully released!");
		ReleaseCOM(m_ShaderResourceView);
		BGS_CORE_TRACE("FRAMEBUFFER::SHADERRESOURCEVIEW succesfully released!");
	}

	void Direct3DFramebuffer::Bind()
	{
		Direct3DContext::GetContext()->GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetView, Direct3DContext::GetContext()->GetDepthStencilView());
	}

	void Direct3DFramebuffer::Unbind()
	{
		Direct3DContext::GetContext()->SetViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void Direct3DFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		// Need to think how to handle this framebuffers, as a part of context? Or another static class storing and manageing render target views.
		if (width == 0 || height == 0 )
		{
			BGS_CORE_WARN("Attempted to rezize framebuffer to (%i, %i)", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	void Direct3DFramebuffer::ClearAttachment(uint32_t attachmentIndex, const math::vec4& color)
	{
		float clearColor[4] = { color.x, color.y, color.z, color.w };
		Direct3DContext::GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView, clearColor);
		//Direct3DContext::GetDeviceContext()->ClearDepthStencilView(Direct3DContext::GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Direct3DFramebuffer::BindTexture(uint32_t slot) const
	{
		Direct3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ShaderResourceView);	
	}

	void Direct3DFramebuffer::UnbindTexture(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		Direct3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void Direct3DFramebuffer::Invalidate()
	{
		ReleaseCOM(m_RenderTargetTexture);
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_ShaderResourceView);

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = m_Specification.Width;
		textureDesc.Height = m_Specification.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = BIGOSTextureFormatToDXGIFormat(m_Specification.TextureFormat);
		textureDesc.SampleDesc.Count = Direct3DContext::GetContext()->IsMSAAEnabled() ? 4 : 1;
		//textureDesc.SampleDesc.Count = Direct3DContext::GetContext()->IsMSAAEnabled() ? (Direct3DContext::GetContext()->GetMSAAQuality() - 1) : 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT hr = Direct3DContext::GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_RenderTargetTexture);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("FRAMEBUFFER::TEXTURE succesfully created!");
	
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		hr = Direct3DContext::GetDevice()->CreateRenderTargetView(m_RenderTargetTexture, &renderTargetViewDesc, &m_RenderTargetView);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("FRAMEBUFFER::RENDERTARGETVIEW succesfully created!");
		
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = Direct3DContext::GetDevice()->CreateShaderResourceView(m_RenderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("FRAMEBUFFER::SHADERRESOURCEVIEW succesfully created!");

		
	}

}