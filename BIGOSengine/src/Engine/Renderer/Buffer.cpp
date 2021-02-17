#include "bgspch.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"

#include "Platform/DirectX/Direct3DBuffer.h"

namespace BIGOS {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECT3D:	return std::make_shared<Direct3DVertexBuffer>(size);				
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
		case RendererAPI::API::DIRECT3D:	return std::make_shared<Direct3DVertexBuffer>(vertices, size);
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
		case RendererAPI::API::DIRECT3D:	return std::make_shared<Direct3DIndexBuffer>(indices, count);
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
		case RendererAPI::API::DIRECT3D:	return std::make_shared<Direct3DConstantBuffer>(size);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}