#include "bgspch.h"
#include "Platform/DX11/DX11Shader.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX11/DX11Common.h"

#include <locale>
#include <string>

#include <d3dcompiler.h>


namespace BIGOS {

	const DX11Shader* DX11Shader::s_CurrentlyBound = nullptr;

	DX11Shader::DX11Shader(const std::string& filepath)
	{
		std::wstring widestr = std::wstring(filepath.begin(), filepath.end());

		HRESULT hr1, hr2;
		ID3DBlob* errorBlob = nullptr;
		hr1 = D3DCompileFromFile(widestr.c_str(), nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_Data.vs, &errorBlob);
		if (SUCCEEDED(hr1))
			BGS_CORE_TRACE("Vertex shader succesfuly created");
		if (errorBlob)
		{
			if (errorBlob->GetBufferSize())
				BGS_CORE_FATAL("Shader Compile Errors: %s", (const char*)errorBlob->GetBufferPointer());
			ReleaseCOM(errorBlob);
		}
		hr2 = D3DCompileFromFile(widestr.c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_Data.ps, &errorBlob);
		if (SUCCEEDED(hr2))
			BGS_CORE_TRACE("Pixel shader succesfuly created");
		if (errorBlob)
		{
			if (errorBlob->GetBufferSize())
				BGS_CORE_FATAL("Shader Compile Errors: %s", (const char*)errorBlob->GetBufferPointer());
			ReleaseCOM(errorBlob);
		}
		DX11Context::GetDevice()->CreateVertexShader(m_Data.vs->GetBufferPointer(), m_Data.vs->GetBufferSize(), nullptr, &m_Data.vertexShader);
		DX11Context::GetDevice()->CreatePixelShader(m_Data.ps->GetBufferPointer(), m_Data.ps->GetBufferSize(), nullptr, &m_Data.pixelShader);

		if (!s_CurrentlyBound)
		{
			Bind();
		}
	}

	DX11Shader::~DX11Shader()
	{
		ReleaseCOM(m_Data.vs);
		ReleaseCOM(m_Data.vertexShader);
		BGS_CORE_TRACE("Vertex shader succesfuly released");
		ReleaseCOM(m_Data.ps);
		ReleaseCOM(m_Data.pixelShader);
		BGS_CORE_TRACE("Pixel shader succesfuly released");
	}

	void DX11Shader::Bind() const
	{
		s_CurrentlyBound = this;

		DX11Context::GetDeviceContext()->VSSetShader(m_Data.vertexShader, NULL, 0);
		DX11Context::GetDeviceContext()->PSSetShader(m_Data.pixelShader, NULL, 0);
	}

	void DX11Shader::Unbind() const
	{

	}
	
}
