#include "bgspch.h"
#include "Engine/Renderer/API/Texture.h"
#include "Engine/Renderer/API/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/Core.h"

#include "Platform/DX11/DX11Texture.h"

namespace BIGOS {

	std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11Texture2D>(width, height);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11Texture2D>(path);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::string* files)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11TextureCube>(files);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	
	}

	std::shared_ptr<TextureCube> TextureCube::Create(const std::string file)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");	return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	return nullptr;
		case RendererAPI::API::DIRECTX11:	return std::make_shared<DX11TextureCube>(file);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	return nullptr;
		}

		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}
}