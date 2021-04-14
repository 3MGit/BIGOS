#include "bgspch.h"
#include "Engine/Renderer/API/RendererAPI.h"
#include "Engine/Core/Core.h"
#include "Platform/DX11/DX11RendererAPI.h"

namespace BIGOS {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DIRECTX11;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!"); return nullptr;
			case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!"); return nullptr;
			case RendererAPI::API::DIRECTX11:	return std::make_unique<DX11RendererAPI>();
			case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!"); return nullptr;
		}
		BGS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}