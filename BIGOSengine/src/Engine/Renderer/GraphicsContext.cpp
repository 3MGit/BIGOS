#include "bgspch.h"
#include "Engine/Renderer/GraphicsContext.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/DirectX/Direct3DContext.h"

namespace BIGOS {

	GraphicsContext* GraphicsContext::s_Context = nullptr;

	void GraphicsContext::Create(WindowProps properties, void* deviceContext)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!");	break;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	break;
		case RendererAPI::API::DIRECT3D:	s_Context = new Direct3DContext(properties, deviceContext);					break;
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	break;
		}
	}
}