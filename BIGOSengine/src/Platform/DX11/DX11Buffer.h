#pragma once
#include "Engine/Renderer/API/Buffer.h"
#include <d3d11.h>

namespace BIGOS {

	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(uint32_t size);
		DX11VertexBuffer(const void* vertices, uint32_t size);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual const void* GetData() override { return m_MappedSubresource.pData; }

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; TranslateLayout(); }
	private:
		void Resize(uint32_t size);
		void TranslateLayout();
	private:
		uint32_t m_Size;
		BufferLayout m_Layout;

		D3D11_BUFFER_DESC m_BufferDesc;
		ID3D11Buffer* m_BufferHandle;
		D3D11_MAPPED_SUBRESOURCE m_MappedSubresource;
		ID3D11InputLayout* m_InputLayout;
	};

	class DX11IndexBuffer: public IndexBuffer
	{
	public:
		DX11IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		ID3D11Buffer* m_BufferHandle;
		uint32_t m_Count;
	};

	class DX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DX11ConstantBuffer(uint32_t size);
		~DX11ConstantBuffer();

		void SetData(const void* data, uint32_t size) override;

		void Bind(uint32_t slot) const override;
		void Unbind() const override;
	private:
		uint32_t m_Size;

		D3D11_BUFFER_DESC m_BufferDesc;
		ID3D11Buffer* m_BufferHandle;
		D3D11_MAPPED_SUBRESOURCE m_MappedSubresource;
	};
}