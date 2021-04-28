#pragma once

#include "Engine/Renderer/API/Shader.h"
#include "Engine/Renderer/API/Buffer.h"
#include "Engine/Renderer/API/Texture.h"
#include "Engine/math/vec4.h"

namespace BIGOS {

	__declspec(align(16))
	struct MaterialUniform
	{
		math::vec4 Albedo;
		float usingAlbedoMap;
		float Metalic;
		float usingMetalicMap;
		float Roughness;
		float usingRoughnessMap;
		float AO;
		float usingAOMap;
		float usingNormalMap;

		MaterialUniform() 
		{
			Albedo = { 0.5f, 0.0f, 0.0f, 1.0f };
			usingAlbedoMap = 0.0f;
			Metalic = 0.0f;
			usingMetalicMap = 0.0f;
			Roughness = 0.2;
			usingRoughnessMap = 0.0f;
			AO = 1.0f;
			usingAOMap = 0.0f;
			usingNormalMap = 0.0f;
		}
	};

	class Material
	{
	public:
		Material(std::shared_ptr<Shader> shader);
		~Material() = default;

		void Bind();
		void Unbind();

		void SetAlbedo(const math::vec4& albedo);
		void SetAlbedoTexture(std::shared_ptr<Texture2D> albedoTextuere);

		void SetMetalic(float metalic);
		void SetMetalicTexture(std::shared_ptr<Texture2D> metalicTextuere);

		void SetRoughness(float roughness);
		void SetRoughnessTexture(std::shared_ptr<Texture2D> roughnessTextuere);

		void SetAO(float ao);
		void SetAOTexture(std::shared_ptr<Texture2D> aoTexture);

		void SetNormals();
		void SetNormalTexture(std::shared_ptr<Texture2D> normalTexture);

		MaterialUniform GetMaterialUniform() const { return m_MaterialData; }
	private:
		MaterialUniform m_MaterialData;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<ConstantBuffer> m_MaterialBuffer;
		std::unordered_map<std::string,std::shared_ptr<Texture2D>> m_Textures;
	};

	__declspec(align(16))
		struct PhongMaterial
	{
		math::vec4 Ambient;
		math::vec4 Diffuse;
		math::vec4 Specular;

		PhongMaterial()
		{

		}

		PhongMaterial(math::vec4 ambient, math::vec4 diffuse, math::vec4 specular)
			: Ambient(ambient), Diffuse(diffuse), Specular(specular) {}
	};

}