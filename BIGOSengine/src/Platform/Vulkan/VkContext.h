#pragma once

#define VOLK_IMPLEMENTATION
#include "Platform/Vulkan/volk.h"

#include "Engine/Renderer/API/GraphicsContext.h"

namespace BIGOS {

	class VkContext : public GraphicsContext
	{
	public:
		VkContext(WindowProps properties, void* deviceContext);

		void Init();
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;
	};

}