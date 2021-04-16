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

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, const math::vec4& color) override;

		// TEMP
		virtual void* GetTexture() const override { return (void*)m_ShaderResourceView; }
		virtual void BindTexture(uint32_t slot) const override;
		virtual void UnbindTexture(uint32_t slot) const override;

		virtual const FramebufferSpecification& GetSpecification() const { return m_Specification; };
	private:
		void Invalidate();
	private:
		FramebufferSpecification m_Specification;

		ID3D11Texture2D* m_RenderTargetTexture;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11ShaderResourceView* m_ShaderResourceView;
	};
}
