#pragma once
#include "Engine/math/vec4.h"

namespace BIGOS {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,

		//Depth/stencil
		DEPTH24STENCIL8,
	};
	
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			:TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::RGBA8;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool CubeFramebuffer = false;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind(uint32_t attachmentSlot) = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, const math::vec4& color) = 0;
		virtual void ClearDepthAttachment() = 0;

		virtual void* GetTexture(uint32_t slot) const = 0;
		virtual void BindTexture(uint32_t slot, uint32_t attachmentSlot) const = 0;
		virtual void UnbindTexture(uint32_t slot) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}