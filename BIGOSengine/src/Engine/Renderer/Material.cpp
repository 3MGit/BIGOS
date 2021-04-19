#include "bgspch.h"
#include "Material.h"

namespace BIGOS
{

	Material::Material(std::shared_ptr<Shader> shader)
		: m_Shader(shader)
	{
		m_MaterialBuffer = ConstantBuffer::Create(sizeof(MaterialUniform));
		m_MaterialData = MaterialUniform();
		m_Textures.resize(3);
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (m_MaterialData.usingAlbedoMap && m_MaterialData.usingMetalicMap && m_MaterialData.usingRoughnessMap)
		{
			for (uint32_t i = 0; i < m_Textures.size(); i++)
			{
				m_Textures[i]->Bind(i);
			}
		}
		m_MaterialBuffer->SetData(&m_MaterialData, sizeof(m_MaterialData));
		m_MaterialBuffer->Bind(2);
	}

	void Material::Unbind()
	{
		if (m_MaterialData.usingAlbedoMap && m_MaterialData.usingMetalicMap && m_MaterialData.usingRoughnessMap)
		{
			for (uint32_t i = 0; i < m_Textures.size(); i++)
			{
				m_Textures[i]->Unbind(i);
			}
		}
	}

	void Material::SetAlbedo(const math::vec4& albedo)
	{
		m_MaterialData.usingAlbedoMap = 0.0f;
		m_MaterialData.Albedo = albedo;
	}

	void Material::SetAlbedoTexture(std::shared_ptr<Texture2D> albedoTextuere)
	{
		m_MaterialData.usingAlbedoMap = 1.0f;
		m_Textures[0] = std::move(albedoTextuere);
	}

	void Material::SetMetalic(float metalic)
	{
		m_MaterialData.usingMetalicMap = 0.0f;
		m_MaterialData.Metalic = metalic;
	}

	void Material::SetMetalicTexture(std::shared_ptr<Texture2D> metalicTextuere)
	{
		m_MaterialData.usingMetalicMap = 1.0f;
		m_Textures[1] = std::move(metalicTextuere);
	}

	void Material::SetRoughness(float roughness)
	{
		m_MaterialData.usingRoughnessMap = 0.0f;
		m_MaterialData.Roughness = roughness;
	}

	void Material::SetRoughnessTexture(std::shared_ptr<Texture2D> roughnessTextuere)
	{
		m_MaterialData.usingRoughnessMap = 1.0f;
		m_Textures[2] = std::move(roughnessTextuere);
	}

	void Material::SetAO(float ao)
	{
		m_MaterialData.AO = ao;
	}

}