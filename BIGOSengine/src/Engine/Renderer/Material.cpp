#include "bgspch.h"
#include "Material.h"

namespace BIGOS
{

	Material::Material(std::shared_ptr<Shader> shader)
		: m_Shader(shader)
	{
		m_MaterialBuffer = ConstantBuffer::Create(sizeof(MaterialUniform));
		m_MaterialData = MaterialUniform();
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		for (auto& texture : m_Textures)
		{
			texture->Bind();
		}

		m_MaterialBuffer->SetData(&m_MaterialData, sizeof(MaterialUniform));
		m_MaterialBuffer->Bind(2);
	}

	void Material::Unbind()
	{
		for (auto& texture : m_Textures)
		{
			texture->Unbind();
		}
	}

}