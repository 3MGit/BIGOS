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
	
	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::RGBA8;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, const math::vec4& color) = 0;

		virtual void* GetTexture() const = 0;
		virtual void BindTexture(uint32_t slot) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}