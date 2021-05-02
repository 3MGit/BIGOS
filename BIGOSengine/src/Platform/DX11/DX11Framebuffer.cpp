#include "bgspch.h"
#include "Platform/DX11/DX11Framebuffer.h"
#include "Engine/Core/Core.h"
#include "Platform/DX11/DX11Common.h"
#include "Platform/DX11/DX11Context.h"

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

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	DX11Framebuffer::DX11Framebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		m_DepthStencilTexture = nullptr;
		m_DepthStencilView = nullptr;

		for (auto format : m_Specification.Attachments.Attachments)
		{
			if (!IsDepthFormat(format.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(format);
			else
				m_DepthAttachmentSpecification = format;
		}

		for (size_t i = 0; i < m_Specification.Attachments.Attachments.size(); ++i)
		{
			m_RenderTargetTextures.resize(m_ColorAttachmentSpecifications.size());
			m_RenderTargetViews.resize(m_ColorAttachmentSpecifications.size());
			m_ShaderResourceViews.resize(m_ColorAttachmentSpecifications.size());
			m_RenderTargetTextures[i] = nullptr;
			m_RenderTargetViews[i] = nullptr;
			m_ShaderResourceViews[i] = nullptr;
		}

		Invalidate();
	}

	DX11Framebuffer::~DX11Framebuffer()
	{
		for (size_t i = 0; i < m_Specification.Attachments.Attachments.size(); ++i)
		{
			ReleaseCOM(m_RenderTargetTextures[i]);
			BGS_CORE_TRACE("FRAMEBUFFER::TEXTURE succesfully released!");
			ReleaseCOM(m_RenderTargetViews[i]);
			BGS_CORE_TRACE("FRAMEBUFFER::RENDERTARGETVIEW succesfully released!");
			ReleaseCOM(m_ShaderResourceViews[i]);
			BGS_CORE_TRACE("FRAMEBUFFER::SHADERRESOURCEVIEW succesfully released!");
		}
		
	}

	void DX11Framebuffer::Bind(uint32_t attachmentSlot)
	{
		DX11Context::GetContext()->GetDeviceContext()->OMSetRenderTargets(1, &m_RenderTargetViews[attachmentSlot], DX11Context::GetContext()->GetDepthStencilView());
	}

	void DX11Framebuffer::Unbind()
	{
		DX11Context::GetContext()->SetViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void DX11Framebuffer::Resize(uint32_t width, uint32_t height)
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

	void DX11Framebuffer::ClearColorAttachment(uint32_t attachmentIndex, const math::vec4& color)
	{
		float clearColor[4] = { color.x, color.y, color.z, color.w };
		DX11Context::GetDeviceContext()->ClearRenderTargetView(m_RenderTargetViews[attachmentIndex], clearColor);		
	}

	void DX11Framebuffer::ClearDepthAttachment()
	{
		if(m_DepthStencilView)
			DX11Context::GetDeviceContext()->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11Framebuffer::BindTexture(uint32_t slot, uint32_t attachmentSlot) const
	{
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ShaderResourceViews[attachmentSlot]);
	}

	void DX11Framebuffer::UnbindTexture(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void DX11Framebuffer::Invalidate()
	{
		for (size_t i = 0; i < m_Specification.Attachments.Attachments.size(); ++i)
		{
			ReleaseCOM(m_RenderTargetTextures[i]);
			ReleaseCOM(m_RenderTargetViews[i]);
			ReleaseCOM(m_ShaderResourceViews[i]);
			m_RenderTargetTextures.resize(m_ColorAttachmentSpecifications.size());
			m_RenderTargetViews.resize(m_ColorAttachmentSpecifications.size());
			m_ShaderResourceViews.resize(m_ColorAttachmentSpecifications.size());
		}

		ReleaseCOM(m_DepthStencilTexture);
		ReleaseCOM(m_DepthStencilView);
		if (m_ColorAttachmentSpecifications.size())
		{
			for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); ++i)
			{

				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));
				textureDesc.Width = m_Specification.Width;
				textureDesc.Height = m_Specification.Height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = BIGOSTextureFormatToDXGIFormat(m_ColorAttachmentSpecifications[0].TextureFormat);
				textureDesc.SampleDesc.Count = DX11Context::GetContext()->IsMSAAEnabled() ? 4 : 1;
				//textureDesc.SampleDesc.Count = Direct3DContext::GetContext()->IsMSAAEnabled() ? (Direct3DContext::GetContext()->GetMSAAQuality() - 1) : 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;

				HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_RenderTargetTextures[i]);
				if (SUCCEEDED(hr))
					BGS_CORE_TRACE("FRAMEBUFFER::TEXTURE succesfully created!");

				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
				ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
				renderTargetViewDesc.Format = textureDesc.Format;
				renderTargetViewDesc.ViewDimension = DX11Context::GetContext()->IsMSAAEnabled() ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
				hr = DX11Context::GetDevice()->CreateRenderTargetView(m_RenderTargetTextures[i], &renderTargetViewDesc, &m_RenderTargetViews[i]);
				if (SUCCEEDED(hr))
					BGS_CORE_TRACE("FRAMEBUFFER::RENDERTARGETVIEW succesfully created!");

				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
				ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
				shaderResourceViewDesc.Format = textureDesc.Format;
				shaderResourceViewDesc.ViewDimension = DX11Context::GetContext()->IsMSAAEnabled() ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
				shaderResourceViewDesc.Texture2D.MipLevels = 1;
				hr = DX11Context::GetDevice()->CreateShaderResourceView(m_RenderTargetTextures[i], &shaderResourceViewDesc, &m_ShaderResourceViews[i]);
				if (SUCCEEDED(hr))
					BGS_CORE_TRACE("FRAMEBUFFER::SHADERRESOURCEVIEW succesfully created!");
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			// Creating DepthStencil buffer
			D3D11_TEXTURE2D_DESC depthStencilDesc;
			depthStencilDesc.Width = m_Specification.Width ? m_Specification.Width : 1;
			depthStencilDesc.Height = m_Specification.Height ? m_Specification.Height : 1;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			depthStencilDesc.SampleDesc.Count = DX11Context::GetContext()->IsMSAAEnabled() ? 4 : 1;
			//depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;

			HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilTexture);
			if (SUCCEEDED(hr))
				BGS_CORE_TRACE("FRAMEBUFFER::DEPTHTEXTURE succesfully created!");
			DX11Context::GetDevice()->CreateDepthStencilView(m_DepthStencilTexture, 0, &m_DepthStencilView);
		}
	}

}