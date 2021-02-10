#pragma once

#include "Engine/Renderer/Shader.h"
#include <d3d11.h>

namespace BIGOS {

	class Direct3DShader : public Shader
	{
	public:
		Direct3DShader(const std::string& filepath);
		//virtual ~Direct3DShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }
		
		static const Direct3DShader* CurrentlyBound() { return s_CurrentlyBound; }
	private:
		std::string m_Name;

		struct Data
		{
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;
			ID3DBlob* vs;
			ID3DBlob* ps;
		};

		mutable Data m_Data;
		static const Direct3DShader* s_CurrentlyBound;
	public:
		inline Data& GetData() const { return m_Data; }
	};

}