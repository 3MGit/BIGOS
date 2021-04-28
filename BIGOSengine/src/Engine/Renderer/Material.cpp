#include "bgspch.h"
#include "Material.h"

namespace BIGOS
{

	Material::Material(std::shared_ptr<Shader> shader)
		: m_Shader(shader)
	{
		m_MaterialBuffer = ConstantBuffer::Create(sizeof(MaterialUniform));
		m_MaterialData = MaterialUniform();
		m_Textures["albedo"] = nullptr;
		m_Textures["metalic"] = nullptr;
		m_Textures["roughness"] = nullptr;
		m_Textures["ao"] = nullptr;
		m_Textures["normal"] = nullptr;
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (m_MaterialData.usingAlbedoMap)
			m_Textures["albedo"]->Bind(0);
		if (m_MaterialData.usingMetalicMap)
			m_Textures["metalic"]->Bind(1);
		if (m_MaterialData.usingRoughnessMap)
			m_Textures["roughness"]->Bind(2);
		if (m_MaterialData.usingAOMap)
			m_Textures["ao"]->Bind(3);
		if (m_MaterialData.usingNormalMap)
			m_Textures["normal"]->Bind(4);

		
		m_MaterialBuffer->SetData(&m_MaterialData, sizeof(m_MaterialData));
		m_MaterialBuffer->Bind(2);
	}

	void Material::Unbind()
	{
		if (m_MaterialData.usingAlbedoMap)
			m_Textures["albedo"]->Unbind(0);
		if (m_MaterialData.usingMetalicMap)
			m_Textures["metalic"]->Unbind(1);
		if (m_MaterialData.usingRoughnessMap)
			m_Textures["roughness"]->Unbind(2);
		if (m_MaterialData.usingAOMap)
			m_Textures["ao"]->Unbind(3);
		if (m_MaterialData.usingNormalMap)
			m_Textures["normal"]->Unbind(4);
	}

	void Material::SetAlbedo(const math::vec4& albedo)
	{
		m_MaterialData.usingAlbedoMap = 0.0f;
		m_MaterialData.Albedo = albedo;
	}

	void Material::SetAlbedoTexture(std::shared_ptr<Texture2D> albedoTextuere)
	{
		m_MaterialData.usingAlbedoMap = 1.0f;
		m_Textures["albedo"] = std::move(albedoTextuere);
	}

	void Material::SetMetalic(float metalic)
	{
		m_MaterialData.usingMetalicMap = 0.0f;
		m_MaterialData.Metalic = metalic;
	}

	void Material::SetMetalicTexture(std::shared_ptr<Texture2D> metalicTextuere)
	{
		m_MaterialData.usingMetalicMap = 1.0f;
		m_Textures["metalic"] = std::move(metalicTextuere);
	}

	void Material::SetRoughness(float roughness)
	{
		m_MaterialData.usingRoughnessMap = 0.0f;
		m_MaterialData.Roughness = roughness;
	}

	void Material::SetRoughnessTexture(std::shared_ptr<Texture2D> roughnessTextuere)
	{
		m_MaterialData.usingRoughnessMap = 1.0f;
		m_Textures["roughness"] = std::move(roughnessTextuere);
	}

	void Material::SetAO(float ao)
	{
		m_MaterialData.usingAOMap = 0.0f;
		m_MaterialData.AO = ao;
	}

	void Material::SetAOTexture(std::shared_ptr<Texture2D> aoTexture)
	{
		m_MaterialData.usingAOMap = 1.0f;
		m_Textures["ao"] = std::move(aoTexture);
	}

	void Material::SetNormals()
	{
		m_MaterialData.usingNormalMap = 0.0f;
	}

	void Material::SetNormalTexture(std::shared_ptr<Texture2D> normalTexture)
	{
		m_MaterialData.usingNormalMap = 1.0f;
		m_Textures["normal"] = std::move(normalTexture);
	}

}