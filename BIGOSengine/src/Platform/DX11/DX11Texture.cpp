#include "bgspch.h"
#include "Platform/DX11/DX11Texture.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX11/DX11Common.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/App/Application.h"
#include "Engine/System/Memory.h"
#include "Engine/Renderer/API/Shader.h"


#include <stb_image.h>

namespace BIGOS {

	namespace Utils {

		static unsigned char* LoadTextureImage(const std::string& path, uint32_t* width, uint32_t* height, int* channels, DXGI_FORMAT* format)
		{
			//forcing texture with 4 channels, do I need to fix it?
			size_t size;
			uint8_t* result;
			stbi_set_flip_vertically_on_load(0);
			stbi_uc* data = nullptr;
			{
				BGS_CORE_TRACE("Stbi loading texture from: {0}", path.c_str());
				data = stbi_load(path.c_str(), (int*)width, (int*)height, channels, STBI_rgb_alpha);
				if (data)
				{
					size = (*width) * (*height) * 4;
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

		static float* LoadHDRImage(const std::string& path, uint32_t* width, uint32_t* height, int* nrComponents, DXGI_FORMAT* format)
		{
			size_t size;
			float* result;
			stbi_set_flip_vertically_on_load(1);
			BGS_CORE_TRACE("Stbi loading HDR texture from: {0}", path.c_str());
			float* data;
			data = stbi_loadf(path.c_str(), (int*)width, (int*)height, nrComponents, 0);
			if (data)
			{
				size = (*width) * (*height) * 4;
				result = bigos_new float[size];
				BGS_CORE_TRACE("{0}Image loaded!", '\t');
			}
			BGS_CORE_ASSERT(data, "Failed to load image!");

			*format = DXGI_FORMAT_R32G32B32A32_FLOAT;

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
		
#if 0

		int channels;
		unsigned char* data = Utils::LoadTextureImage(path, &m_Width, &m_Height, &channels, &m_Format);

		uint32_t fmtSupport = 0;
		DX11Context::GetDevice()->CheckFormatSupport(m_Format, &fmtSupport);
		BGS_CORE_ASSERT(fmtSupport, "DirectX cannot support this format");

		//Testing creation textures from *.tga files
		channels = 4;

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
#endif

		LoadTexture();
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

	void DX11Texture2D::LoadTexture()
	{
		BGS_CORE_TRACE("Loading texture from: {0}", m_Path.c_str());
		int width, height, channels;
		void* data = nullptr;
		if (stbi_is_hdr(m_Path.c_str()))
		{
			float* pixels = stbi_loadf(m_Path.c_str(), &width, &height, &channels, 4);
			data = (void*)pixels;
			m_IsHDR = true;
		}
		else
		{
			stbi_uc* pixels = stbi_load(m_Path.c_str(), &width, &height, &channels, 4);
			data = (void*)pixels;
			m_IsHDR = false;
		}

		if (data == nullptr)
		{
			BGS_CORE_ERROR("Failed to load image from filepath {0}'!", m_Path.c_str());
			return;
		}

		m_Width = width;
		m_Height = height;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = m_Width;
		textureDesc.Height = m_Height;
		if (!m_IsHDR)
			textureDesc.MipLevels = 1;
		else
			textureDesc.MipLevels = 1;

		textureDesc.ArraySize = 1;

		if (m_IsHDR)
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		else
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		/*
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = 4 * m_Width;
		initData.SysMemSlicePitch = m_Width * m_Height * 4;

		D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;
		if (data) initDataPtr = &initData;
		*/
		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_Texture);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11Texture2D succesfully created!");

		
		uint32_t rowPitch;
		if (!m_IsHDR)
			rowPitch = m_Width * 4 * sizeof(unsigned char);
		else
			rowPitch = m_Width * 4 * sizeof(float);

		DX11Context::GetDeviceContext()->UpdateSubresource(m_Texture, 0, 0, data, rowPitch, 0);
		

		//Create the Shader Resource View
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		hr = DX11Context::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ResourceView);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11ShaderResourceView succesfully created!");

		//Create sampler state
		ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));
		//m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		//m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.MinLOD = 0;
		m_SamplerDesc.MaxLOD = 11;
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = DX11Context::GetDevice()->CreateSamplerState(&m_SamplerDesc, &m_SamplerState);
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11SamplerState succesfully created!");

		free(data);
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

	DX11TextureCube::DX11TextureCube(const std::string file)
		: m_HdrPath(file)
	{
		auto captureProjection = math::mat4::Perspective(90.0f, 1.0f, 0.1f, 10.0f);

		m_CaptureViewProjection =
		{
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(1.0f,  0.0f,  0.0f), math::vec3(0.0f, -1.0f,  0.0f)),
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(-1.0f, 0.0f,  0.0f), math::vec3(0.0f, -1.0f,  0.0f)),
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f,  1.0f,  0.0f), math::vec3(0.0f,  0.0f,  1.0f)),
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f, -1.0f,  0.0f), math::vec3(0.0f,  0.0f, -1.0f)),
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f,  0.0f,  1.0f), math::vec3(0.0f, -1.0f,  0.0f)),
			captureProjection * math::mat4::LookAt(math::vec3(0.0f, 0.0f, 0.0f), math::vec3(0.0f,  0.0f, -1.0f), math::vec3(0.0f, -1.0f,  0.0f)),
		};

		LoadFromHDR();

		//GenerateIrradiance();
	}

	DX11TextureCube::~DX11TextureCube()
	{
		ReleaseCOM(m_SamplerState);
		BGS_CORE_TRACE("D3D11SamplerState succesfully released!");
		ReleaseCOM(m_ShaderResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_IrradianceShaderResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_PrefilteredShaderResourceView);
		BGS_CORE_TRACE("D3D11ShaderResourceView succesfully released!");
		ReleaseCOM(m_Texture);
		BGS_CORE_TRACE("D3D11TextureCube succesfully released!");
		if(m_Files)
			delete[] m_Files;
		
	}

	void DX11TextureCube::SetData(void* data, uint32_t size)
	{
	}

	void DX11TextureCube::Bind(uint32_t slot) const
	{
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_ShaderResourceView);
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

		hr = DX11Context::GetDevice()->CreateShaderResourceView(m_Texture, &srvDesc, &m_ShaderResourceView);
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

	void DX11TextureCube::LoadFromHDR()
	{
		__declspec(align(16))
			struct SkyboxConstantBufferData
		{
			math::mat4 u_ModelViewProj;
		};
		
		ScopedTimer timer;
		auto texture = Texture2D::Create(m_HdrPath);
		auto shader = Shader::Create("assets/shaders/HDRtoCube.hlsl");
		auto cubeCB = ConstantBuffer::Create(sizeof(SkyboxConstantBufferData));

		m_Cube = MeshGenerator::CreateBox({ 2.0f, 2.0f, 2.0f });

		uint32_t width = 1024;
		uint32_t height = 1024;

		//Create the TextureCube
		ID3D11Texture2D* tex = nullptr;
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &tex);

		//Shader Resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		hr = DX11Context::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_ShaderResourceView);

		//Create the Render target views
		ID3D11RenderTargetView* rtvs[6];
		for (uint32_t i = 0; i < 6; i++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = 0;
			renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
			renderTargetViewDesc.Texture2DArray.ArraySize = 1;
			//ID3D11RenderTargetView* view = nullptr;
			DX11Context::GetDevice()->CreateRenderTargetView(tex, &renderTargetViewDesc, &rtvs[i]);
		}

		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		DX11Context::GetDeviceContext()->RSSetViewports(1, &viewport);

		texture->Bind(0);
		shader->Bind();

		for (uint32_t i = 0; i < 6; i++)
		{
			float col[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			DX11Context::GetDeviceContext()->ClearRenderTargetView(rtvs[i], col);
			DX11Context::GetDeviceContext()->OMSetRenderTargets(1, &rtvs[i], nullptr);
			math::mat4 tempSkyboxTransform = BIGOS::math::mat4::Translate({ 0.0f, 0.0f, 0.0f });
			SkyboxConstantBufferData dataCB;
			dataCB.u_ModelViewProj = BIGOS::math::mat4::Invert(m_CaptureViewProjection[i]);
			//dataCB.u_ModelViewProj = m_CaptureViewProjection[i];
			cubeCB->SetData(&dataCB, sizeof(dataCB));
			cubeCB->Bind(0);
			m_Cube->Render();
		}

		DX11Context::GetDeviceContext()->GenerateMips(m_ShaderResourceView);

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

		//DX11Context::GetDeviceContext()->OMSetRenderTargets(1, DX11Context::GetRenderTargetView(), nullptr);

		DX11Context::GetContext()->SetViewport(0, 0, Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());

		for (ID3D11RenderTargetView*& rtv : rtvs)
			rtv->Release();
		tex->Release();
		delete m_Cube;
		BGS_CORE_INFO("HDR to Cubemap conversion took {0} seconds", (timer.ElapsedMillis() / 1000));
		
	}
	void DX11TextureCube::GenerateIrradianceMap()
	{
		__declspec(align(16))
		struct SkyboxConstantBufferData
		{
			math::mat4 u_ModelViewProj;
		};

		ScopedTimer timer;
		uint32_t width = 32;
		uint32_t height = 32;

		auto shader = Shader::Create("assets/shaders/IrradianceConvolution.hlsl");
		auto cubeCB = ConstantBuffer::Create(sizeof(SkyboxConstantBufferData));

		m_Cube = MeshGenerator::CreateBox({ 2.0f, 2.0f, 2.0f });

		//Create the TextureCube
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS; // XD
		ID3D11Texture2D* tex = nullptr;
		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &tex);

		//Shader Resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr = DX11Context::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_IrradianceShaderResourceView);

		//Create the Render target views
		ID3D11RenderTargetView* rtvs[6];
		for (uint32_t i = 0; i < 6; i++)
		{
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			renderTargetViewDesc.Texture2DArray.MipSlice = 0;
			renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
			renderTargetViewDesc.Texture2DArray.ArraySize = 1;
			DX11Context::GetDevice()->CreateRenderTargetView(tex, &renderTargetViewDesc, &rtvs[i]);
		}

		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		DX11Context::GetDeviceContext()->RSSetViewports(1, &viewport);

		shader->Bind();
		DX11Context::GetDeviceContext()->PSSetShaderResources(5, 1, &m_ShaderResourceView);
		DX11Context::GetDeviceContext()->PSSetSamplers(5, 1, &m_SamplerState);
		

		for (uint32_t i = 0; i < 6; i++)
		{
			float col[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			DX11Context::GetDeviceContext()->ClearRenderTargetView(rtvs[i], col);
			DX11Context::GetDeviceContext()->OMSetRenderTargets(1, &rtvs[i], nullptr);
			math::mat4 tempSkyboxTransform = BIGOS::math::mat4::Translate({ 0.0f, 0.0f, 0.0f });
			SkyboxConstantBufferData dataCB;
			dataCB.u_ModelViewProj = BIGOS::math::mat4::Invert(m_CaptureViewProjection[i]);
			//dataCB.u_ModelViewProj = m_CaptureViewProjection[i];
			cubeCB->SetData(&dataCB, sizeof(dataCB));
			cubeCB->Bind(0);
			m_Cube->Render();
		}

		//Cleanup

		DX11Context::GetContext()->SetViewport(0, 0, Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());

		tex->Release();
		for (ID3D11RenderTargetView*& rtv : rtvs)
			rtv->Release();

		delete m_Cube;

		BGS_CORE_INFO("Irradiance map generation took {0} seconds", (timer.ElapsedMillis() / 1000));
	}

	void DX11TextureCube::GeneratePrefilteredMap()
	{
		__declspec(align(16))
		struct SkyboxConstantBufferData
		{
			math::mat4 u_InvertViewProj;
			math::vec4 u_Roughness;
		};

		ScopedTimer timer;
		uint32_t width = 128;
		uint32_t height = 128;

		auto shader = Shader::Create("assets/shaders/Prefiltered.hlsl");
		auto cubeCB = ConstantBuffer::Create(sizeof(SkyboxConstantBufferData));

		m_Cube = MeshGenerator::CreateBox({ 2.0f, 2.0f, 2.0f });

		//Create the TextureCube
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 6;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Texture2D* tex = nullptr;
		HRESULT hr = DX11Context::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &tex);
		//Shader Resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;
		DX11Context::GetDevice()->CreateShaderResourceView(tex, &srvDesc, &m_PrefilteredShaderResourceView);

		uint32_t maxMipLevels = 5;
		std::vector<ID3D11RenderTargetView*> rtvs;
		for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
		{
			for (uint32_t i = 0; i < 6; ++i)
			{
				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
				renderTargetViewDesc.Format = textureDesc.Format;
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				renderTargetViewDesc.Texture2DArray.MipSlice = mip;
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
				renderTargetViewDesc.Texture2DArray.ArraySize = 1;
				ID3D11RenderTargetView* view = nullptr;
				DX11Context::GetDevice()->CreateRenderTargetView(tex, &renderTargetViewDesc, &view);
				rtvs.push_back(view);
			}
		}

		for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
		{
			uint32_t mipWidth = static_cast<uint32_t>(width * std::pow(0.5, mip));
			uint32_t mipHeight = static_cast<uint32_t>(height * std::pow(0.5, mip));
			D3D11_VIEWPORT mViewport = {};
			mViewport.TopLeftX = 0.0f;
			mViewport.TopLeftY = 0.0f;
			mViewport.Width = static_cast<float>(mipWidth);
			mViewport.Height = static_cast<float>(mipHeight);
			mViewport.MinDepth = 0.0f;
			mViewport.MaxDepth = 1.0f;
			DX11Context::GetDeviceContext()->RSSetViewports(1, &mViewport);

			shader->Bind();
			DX11Context::GetDeviceContext()->PSSetShaderResources(5, 1, &m_ShaderResourceView);
			DX11Context::GetDeviceContext()->PSSetSamplers(5, 1, &m_SamplerState);

			math::vec4 data = { ((float)mip / (float)(maxMipLevels - 1)), 0.0f, 0.0f, 0.0f };
			for (uint32_t i = 0; i < 6; ++i)
			{
				float col[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
				DX11Context::GetDeviceContext()->ClearRenderTargetView(rtvs[mip * 6 + i], col);
				DX11Context::GetDeviceContext()->OMSetRenderTargets(1, &rtvs[mip * 6 + i], nullptr);
				math::mat4 tempSkyboxTransform = BIGOS::math::mat4::Translate({ 0.0f, 0.0f, 0.0f });
				SkyboxConstantBufferData dataCB;
				dataCB.u_InvertViewProj = BIGOS::math::mat4::Invert(m_CaptureViewProjection[i]);
				//dataCB.u_InvertViewProj = m_CaptureViewProjection[i];
				dataCB.u_Roughness = data;
				cubeCB->SetData(&dataCB, sizeof(dataCB));
				cubeCB->Bind(4);
				m_Cube->Render();
			}
		}

		DX11Context::GetDeviceContext()->GenerateMips(m_PrefilteredShaderResourceView);

		DX11Context::GetContext()->SetViewport(0, 0, Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight());

		//Cleanup
		tex->Release();
		for (auto& rtv : rtvs)
			rtv->Release();
		rtvs.clear();

		delete m_Cube;

		BGS_CORE_INFO("Prefiltered map generation took {0} seconds", (timer.ElapsedMillis() / 1000));
	}

	void DX11TextureCube::BindPrefilteredMap(uint32_t slot) const
	{
		if (m_PrefilteredShaderResourceView)
		{
			DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_PrefilteredShaderResourceView);
			DX11Context::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
		}
	}

	void DX11TextureCube::UnbindPrefilteredMap(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}

	void DX11TextureCube::UnbindIrradianceMap(uint32_t slot) const
	{
		ID3D11ShaderResourceView* rv = nullptr;
		DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &rv);
	}
	void DX11TextureCube::BindIrradianceMap(uint32_t slot) const
	{
		if (m_IrradianceShaderResourceView)
		{
			DX11Context::GetDeviceContext()->PSSetShaderResources(slot, 1, &m_IrradianceShaderResourceView);
			DX11Context::GetDeviceContext()->PSSetSamplers(slot, 1, &m_SamplerState);
		}
	}
}