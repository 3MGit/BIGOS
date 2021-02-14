#include "bgspch.h"
#include "Platform/DirectX/Direct3DShader.h"
#include "Platform/DirectX/Direct3DContext.h"

#include <locale>
#include <string>

#include <d3dcompiler.h>

namespace BIGOS {

	const Direct3DShader* Direct3DShader::s_CurrentlyBound = nullptr;

	Direct3DShader::Direct3DShader(const std::string& filepath)
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
			errorBlob->Release();
		}
		hr2 = D3DCompileFromFile(widestr.c_str(), nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_Data.ps, &errorBlob);
		if (SUCCEEDED(hr2))
			BGS_CORE_TRACE("Pixel shader succesfuly created");
		if (errorBlob)
		{
			if (errorBlob->GetBufferSize())
				BGS_CORE_FATAL("Shader Compile Errors: %s", (const char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		Direct3DContext::GetDevice()->CreateVertexShader(m_Data.vs->GetBufferPointer(), m_Data.vs->GetBufferSize(), nullptr, &m_Data.vertexShader);
		Direct3DContext::GetDevice()->CreatePixelShader(m_Data.ps->GetBufferPointer(), m_Data.ps->GetBufferSize(), nullptr, &m_Data.pixelShader);
	
		
	}

	Direct3DShader::~Direct3DShader()
	{
		m_Data.vs->Release();
		m_Data.vertexShader->Release();
		BGS_CORE_TRACE("Vertex shader succesfuly released");
		m_Data.ps->Release();
		m_Data.pixelShader->Release();
		BGS_CORE_TRACE("Pixel shader succesfuly released");
	}

	void Direct3DShader::Bind() const
	{
		s_CurrentlyBound = this;

		Direct3DContext::GetDeviceContext()->VSSetShader(m_Data.vertexShader, NULL, 0);
		Direct3DContext::GetDeviceContext()->PSSetShader(m_Data.pixelShader, NULL, 0);

		BGS_CORE_TRACE("Shaders bound");
	}

	void Direct3DShader::Unbind() const
	{
	}

}
