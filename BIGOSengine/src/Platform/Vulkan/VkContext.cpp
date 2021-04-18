#include "bgspch.h"
#include "Platform/Vulkan/VkContext.h"

namespace BIGOS {



	VkContext::VkContext(WindowProps properties, void* deviceContext)
	{
	}

	void VkContext::Init()
	{
		VkResult result = volkInitialize();

	}

}