#include "bgspch.h"
#include "Engine/Renderer/LightManager.h"

namespace BIGOS {

	std::vector<Light> LightManager::s_Lights;

	LightManager::~LightManager()
	{

	}

	void LightManager::AddLight(Light light)
	{
		s_Lights.push_back(light);
	}

	void LightManager::AddLight(const math::vec3& position, const math::vec4& color)
	{
		s_Lights.push_back(Light(position,color));
	}

	std::unique_ptr<LightManager> LightManager::Create()
	{
		return std::unique_ptr<LightManager>();
	}

}