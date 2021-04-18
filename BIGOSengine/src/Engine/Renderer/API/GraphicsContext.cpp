#include "bgspch.h"
#include "Engine/Renderer/API/GraphicsContext.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/DX11/DX11Context.h"

namespace BIGOS {

	GraphicsContext* GraphicsContext::s_Context = nullptr;

	void GraphicsContext::Create(WindowProps properties, void* windowHandle, void* windowInstance)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:		BGS_CORE_ASSERT(false, "RendererAPI::NONE is currently not supported!");	break;
		case RendererAPI::API::OPENGL:		BGS_CORE_ASSERT(false, "RendererAPI::OPENGL is currently not supported!");	break;
		case RendererAPI::API::DIRECTX11:	s_Context = new DX11Context(properties, windowHandle, windowInstance);					break;
		case RendererAPI::API::VULKAN:		BGS_CORE_ASSERT(false, "RendererAPI::VULKAN is currently not supported!");	break;
		}
	}
}