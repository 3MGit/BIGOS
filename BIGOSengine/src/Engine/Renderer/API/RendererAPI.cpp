#include "bgspch.h"
#include "Engine/Renderer/API/RendererAPI.h"
#include "Engine/Core/Core.h"
#include "Platform/DirectX/Direct3DRendererAPI.h"

namespace BIGOS {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DIRECT3D;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); return nullptr;
			case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!"); return nullptr;
			case RendererAPI::API::DIRECT3D:	return std::make_unique<Direct3DRendererAPI>();
			case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!"); return nullptr;
		}
		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}