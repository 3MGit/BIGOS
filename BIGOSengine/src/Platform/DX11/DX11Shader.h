#pragma once

#include "Engine/Renderer/API/Shader.h"
#include "Platform/DX11/DX11Buffer.h"
#include <d3d11.h>

namespace BIGOS {

	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const std::string& filepath);
		virtual ~DX11Shader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		static const DX11Shader* CurrentlyBound() { return s_CurrentlyBound; }
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
		static const DX11Shader* s_CurrentlyBound;
	public:
		inline Data& GetData() const { return m_Data; }
	};

}