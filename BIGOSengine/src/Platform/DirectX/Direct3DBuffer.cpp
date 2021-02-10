#include "bgspch.h"
#include "Platform/DirectX/Direct3DBuffer.h"
#include "Direct3DContext.h"

namespace BIGOS {

	static DXGI_FORMAT ShaderDataTypeToDirect3DBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		//case ShaderDataType::Mat3:     return IDK WHAT TO RETURN, AND IF I NEED THAT;
		//case ShaderDataType::Mat4:     return IDK WHAT TO RETURN, AND IF I NEED THAT;
		case ShaderDataType::Int:      return DXGI_FORMAT_R32_UINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_UINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_UINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_UINT;
		//case ShaderDataType::Bool:     return IDK WHAT TO RETURN, AND IF I NEED THAT;
		}

		BGS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	Direct3DVertexBuffer::Direct3DVertexBuffer(uint32_t size)
		: m_Size(size)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		Resize(m_Size);
	}

	Direct3DVertexBuffer::Direct3DVertexBuffer(float* vertices, uint32_t size)
		: m_Size(size)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		SetData((void*)vertices, size);
	}

	Direct3DVertexBuffer::~Direct3DVertexBuffer()
	{
	}

	void Direct3DVertexBuffer::Bind() const
	{
		uint32_t offset = 0;
		uint32_t stride = m_Layout.GetStride();
		Direct3DContext::GetDeviceContext()->IASetInputLayout(m_InputLayout);
		Direct3DContext::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
	}

	void Direct3DVertexBuffer::Unbind() const
	{
	}

	void Direct3DVertexBuffer::SetData(const void* data, uint32_t size)
	{
		Resize(size);

		if (SUCCEEDED(Direct3DContext::GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresource))) ///D3D11_MAP_WRITE_DISCARD
		{
			//throw an exception while writting memmory
			memcpy(m_MappedSubresource.pData, data, size);
			Direct3DContext::GetDeviceContext()->Unmap(m_BufferHandle, NULL);
		}
	}

	void Direct3DVertexBuffer::Resize(uint32_t size)
	{
		HRESULT hr;
		m_Size = size;
		m_BufferDesc.ByteWidth = m_Size;
		hr = Direct3DContext::GetContext()->GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &m_BufferHandle);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11VertexBuffer");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11VertexBuffer succesfully created!");
	}

	void Direct3DVertexBuffer::TranslateLayout()
	{
		HRESULT hr;
		const std::vector<BufferElement>& layout = m_Layout.GetElements();
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[layout.size()];
		for (uint32_t i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			desc[i] = { element.Name.c_str(), 0, ShaderDataTypeToDirect3DBaseType(element.Type), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		// TODO: Change when we add shaders
		hr = Direct3DContext::GetDevice()->CreateInputLayout(desc, layout.size(), NULL, NULL, &m_InputLayout);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11InputLayout");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11InputLayout succesfully created!");

		delete[] desc;
	}

}