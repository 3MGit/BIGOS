#include "bgspch.h"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"

#include "Platform/DirectX/Direct3DShader.h"

namespace BIGOS {

	const Shader* Shader::s_CurrentlyBound = nullptr;

	std::unique_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); return nullptr;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!"); return nullptr;
		case RendererAPI::API::DIRECT3D:	return std::make_unique<Direct3DShader>(filepath);
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!"); return nullptr;
		}
		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}