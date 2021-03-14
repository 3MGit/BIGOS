#include "bgspch.h"
#include "Platform/DirectX/Direct3DTexture.h"
#include "Platform/DirectX/Direct3DContext.h"
#include "Platform/DirectX/Direct3DCommon.h"
#include "Engine/Core/Core.h"

#include <stb_image.h>

namespace BIGOS {

	Direct3DTexture2D::Direct3DTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
	}

	Direct3DTexture2D::Direct3DTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = nullptr;
		{
			BGS_CORE_TRACE("Stbi loading texture from: %s", path.c_str());
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			if(data) BGS_CORE_TRACE("Image loaded!");
		}
		BGS_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		}
		else if (channels == 3)
		{
			m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//BGS_CORE_TRACE("Texture format with %i channels is currently not supported!", channels);
		}

		uint32_t fmtSupport = 0;
		Direct3DContext::GetDevice()->CheckFormatSupport(m_Format, &fmtSupport);
		BGS_CORE_ASSERT(fmtSupport, "DirectX cannot support this format");

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = channels * m_Width;
		initData.SysMemSlicePitch = m_Width * m_Height * channels;

		D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;
		if (data) initDataPtr = &initData;

		ZeroMemory(&m_Desc, sizeof(D3D11_TEXTURE2D_DESC));
		m_Desc.Width = m_Width;
		m_Desc.Height = m_Height;
		m_Desc.MipLevels = 1;
		m_Desc.ArraySize = 1;
		m_Desc.Format = m_Format;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
		m_Desc.CPUAccessFlags = 0;
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.MiscFlags = 0;
		m_Desc.SampleDesc.Count = 1;
		m_Desc.SampleDesc.Quality = 0;

		HRESULT hr = Direct3DContext::GetDevice()->CreateTexture2D(&m_Desc, initDataPtr, &m_Texture);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11Texture2D succesfully created!");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = m_Desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_Desc.MipLevels;

		hr = Direct3DContext::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11ShaderResourceView succesfully created!");
	
		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.MinLOD = 0;
		m_SamplerDesc.MaxLOD = 11;
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = Direct3DContext::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11SamplerState succesfully created!");

		stbi_image_free(data);
	}

	Direct3DTexture2D::~Direct3DTexture2D()
	{
		ReleaseCOM(m_SamplerState);
		BGS_CORE_TRACE("D3D11SamplerState succesfully released!");
		ReleaseCOM(m_ResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_Texture);
		BGS_CORE_TRACE("D3D11Texture2D succesfully released!");
	}

	void Direct3DTexture2D::SetData(void* data, uint32_t size)
	{
	}

	void Direct3DTexture2D::Bind(uint32_t slot) const
	{
		Direct3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		Direct3DContext::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void Direct3DTexture2D::Unbind(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		Direct3DContext::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

}