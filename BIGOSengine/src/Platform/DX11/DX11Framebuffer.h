#pragma once

#include "Engine/Renderer/API/Framebuffer.h"
#include "Engine/math/vec4.h"

#include <d3d11.h>

namespace BIGOS {

	class DX11Framebuffer : public Framebuffer
	{
	public:
		DX11Framebuffer(const FramebufferSpecification& spec);
		~DX11Framebuffer();

		virtual void Bind(uint32_t attachmentSlot) override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, const math::vec4& color) override;
		virtual void ClearDepthAttachment() override;

		// TEMP
		virtual void* GetTexture(uint32_t slot) const override { return (void*)m_ShaderResourceViews[slot]; }
		virtual void BindTexture(uint32_t slot, uint32_t attachmentSlot) const override;
		virtual void UnbindTexture(uint32_t slot) const override;

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; };
	private:
		void Invalidate();
	private:
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		ID3D11Texture2D* m_DepthStencilTexture;
		ID3D11DepthStencilView* m_DepthStencilView;

		std::vector<ID3D11Texture2D*> m_RenderTargetTextures;
		std::vector<ID3D11RenderTargetView*> m_RenderTargetViews;
		std::vector<ID3D11ShaderResourceView*> m_ShaderResourceViews;
	};
}
