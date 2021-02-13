#pragma once
#include "Engine/Renderer/Buffer.h"
#include <d3d11.h>

namespace BIGOS {

	class Direct3DVertexBuffer : public VertexBuffer
	{
	public:
		Direct3DVertexBuffer(uint32_t size);
		Direct3DVertexBuffer(float* vertices, uint32_t size);
		virtual ~Direct3DVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

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

	class Direct3DIndexBuffer: public IndexBuffer
	{
	public:
		Direct3DIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~Direct3DIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		ID3D11Buffer* m_Handle;
		uint32_t m_Count;
	};

}