#include "bgspch.h"
#include "Platform/DX11/DX11Texture.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX11/DX11Common.h"
#include "Engine/Core/Core.h"
#include "Engine/System/Memory.h"

#include <stb_image.h>

namespace BIGOS {

	namespace Utils {

		static unsigned char* LoadTextureImage(const std::string& path, uint32_t* width, uint32_t* height, int* channels, DXGI_FORMAT* format)
		{
			size_t size;
			uint8_t* result;
			stbi_set_flip_vertically_on_load(0);
			stbi_uc* data = nullptr;
			{
				BGS_CORE_TRACE("Stbi loading texture from: {0}", path.c_str());
				data = stbi_load(path.c_str(), (int*)width, (int*)height, channels, 0);
				if (data)
				{
					size = (*width) * (*height) *(*channels);
					result = bigos_new uint8_t[size];
					BGS_CORE_TRACE("{0}Image loaded!", '\t');
				}
			}
			BGS_CORE_ASSERT(data, "Failed to load image!");

			if (*channels == 4)
			{
				*format = DXGI_FORMAT_R8G8B8A8_UNORM;
			}
			else if (*channels == 3)
			{
				*format = DXGI_FORMAT_R8G8B8A8_UNORM;
				//BGS_CORE_TRACE("Texture format with %i channels is currently not supported!", channels);
			}

			memcpy(result, data, size);
			stbi_image_free(data);

			return result;
		}

	}

	DX11Texture2D::DX11Texture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
	}

	DX11Texture2D::DX11Texture2D(const std::string& path)
		: m_Path(path)
	{	
		int chanels;
		unsigned char* data = Utils::LoadTextureImage(path, &m_Width, &m_Height, &chanels, &m_Format);

		uint32_t fmtSupport = 0;
		DX11Context::GetDevice()->CheckFormatSupport(m_Format, &fmtSupport);
		BGS_CORE_ASSERT(fmtSupport, "DirectX cannot support this format");

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = chanels * m_Width;
		initData.SysMemSlicePitch = m_Width * m_Height * chanels;

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

		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&m_Desc, initDataPtr, &m_Texture);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11Texture2D succesfully created!");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = m_Desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_Desc.MipLevels;

		hr = DX11Context::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView);
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

		hr = DX11Context::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11SamplerState succesfully created!");

		bigos_delete[] data;
	}

	DX11Texture2D::~DX11Texture2D()
	{
		ReleaseCOM(m_SamplerState);
		BGS_CORE_TRACE("D3D11SamplerState succesfully released!");
		ReleaseCOM(m_ResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_Texture);
		BGS_CORE_TRACE("D3D11Texture2D succesfully released!");
	}

	void DX11Texture2D::SetData(void* data, uint32_t size)
	{
	}

	void DX11Texture2D::Bind(uint32_t slot) const
	{
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		DX11Context::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void DX11Texture2D::Unbind(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	DX11TextureCube::DX11TextureCube(const std::string* files)
	{
		m_Files = new std::string[6];
		for (int i = 0; i < 6; i++)
		{
			m_Files[i] = files[i];
		}

		LoadFromMultipleFiles();
	}

	DX11TextureCube::~DX11TextureCube()
	{
		ReleaseCOM(m_SamplerState);
		BGS_CORE_TRACE("D3D11SamplerState succesfully released!");
		ReleaseCOM(m_ResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_Texture);
		BGS_CORE_TRACE("D3D11TextureCube succesfully released!");
		delete[] m_Files;
	}

	void DX11TextureCube::SetData(void* data, uint32_t size)
	{
	}

	void DX11TextureCube::Bind(uint32_t slot) const
	{
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ResourceView);
		DX11Context::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	void DX11TextureCube::Unbind(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void DX11TextureCube::LoadFromMultipleFiles()
	{
		unsigned char* data[6];

		int channels;

		for (int i = 0; i < 6; i++)
		{
			data[i] = Utils::LoadTextureImage(m_Files[i], &m_Width, &m_Height, &channels, &m_Format);
			
		}

		ZeroMemory(&m_Desc, sizeof(D3D11_TEXTURE2D_DESC));
		m_Desc.Width = m_Width;
		m_Desc.Height = m_Height;
		m_Desc.MipLevels = 1;
		m_Desc.ArraySize = 6;
		m_Desc.Format = m_Format;
		m_Desc.CPUAccessFlags = 0;
		m_Desc.SampleDesc.Count = 1;
		m_Desc.SampleDesc.Quality = 0;
		m_Desc.Usage = D3D11_USAGE_DEFAULT;
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.CPUAccessFlags = 0;
		m_Desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SUBRESOURCE_DATA initData[6];
		for (int i = 0; i < 6; i++)
		{
			initData[i].pSysMem = data[i];
			initData[i].SysMemPitch = channels * m_Width;
			initData[i].SysMemSlicePitch = m_Width * m_Height * channels;
		}

		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&m_Desc, initData, &m_Texture);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11TextureCube succesfully created!");


		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = m_Desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = m_Desc.MipLevels;
		srvDesc.TextureCube.MostDetailedMip = 0;

		hr = DX11Context::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11ShaderResourceView succesfully created!");

		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = DX11Context::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11SamplerState succesfully created!");

		for(int i = 0; i<6; i++)
			bigos_delete[](data[i]);
	}

}