#pragma once

#include "Engine/Renderer/API/Texture.h"
#include <d3d11.h>

namespace BIGOS {

	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(uint32_t width, uint32_t height);
		DX11Texture2D(const std::string& path);
		~DX11Texture2D();

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;

		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ResourceView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
		D3D11_SAMPLER_DESC m_SamplerDesc;
		DXGI_FORMAT m_Format = DXGI_FORMAT_UNKNOWN;
	};

	class DX11TextureCube : public TextureCube
	{
	public:
		DX11TextureCube(const std::string* files);
		~DX11TextureCube();

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Unbind(uint32_t slot = 0) const override;
	private:
		void LoadFromMultipleFiles();
	private:
		std::string* m_Files;
		uint32_t m_Width, m_Height;

		D3D11_TEXTURE2D_DESC m_Desc;
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11ShaderResourceView* m_ResourceView = nullptr;
		ID3D11SamplerState* m_SamplerState = nullptr;
		D3D11_SAMPLER_DESC m_SamplerDesc;
		DXGI_FORMAT m_Format = DXGI_FORMAT_UNKNOWN;
	};
}
