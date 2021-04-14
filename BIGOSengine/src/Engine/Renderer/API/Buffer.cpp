#include "bgspch.h"
#include "Engine/Renderer/API/Buffer.h"
#include "Engine/Renderer/API/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"

#include "Platform/DX11/DX11Buffer.h"

namespace BIGOS {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11VertexBuffer>(size);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11VertexBuffer>(vertices, size);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11IndexBuffer>(indices, count);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11ConstantBuffer>(size);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}