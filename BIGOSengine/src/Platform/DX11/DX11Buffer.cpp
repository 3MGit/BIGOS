#include "bgspch.h"
#include "Platform/DX11/DX11Buffer.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX11/DX11Shader.h"
#include "Platform/DX11/DX11Common.h"

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

	DX11VertexBuffer::DX11VertexBuffer(uint32_t size)
		: m_Size(size)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		m_BufferHandle = nullptr;

		Resize(m_Size);
	}

	DX11VertexBuffer::DX11VertexBuffer(const void* vertices, uint32_t size)
		: m_Size(size)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		Resize(m_Size);
		SetData(vertices, size);
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		ReleaseCOM(m_InputLayout);
		BGS_CORE_TRACE("D3D11InputLayout succesfully released!");
		ReleaseCOM(m_BufferHandle);
		BGS_CORE_TRACE("D3D11VertexBuffer succesfully released!");
	}

	void DX11VertexBuffer::Bind() const
	{
		uint32_t offset = 0;
		uint32_t stride = m_Layout.GetStride();
		DX11Context::GetDeviceContext()->IASetInputLayout(m_InputLayout);
		DX11Context::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_BufferHandle, &stride, &offset);
	}

	void DX11VertexBuffer::Unbind() const
	{
	}

	void DX11VertexBuffer::SetData(const void* data, uint32_t size)
	{
		if(size>m_Size)
			Resize(size);
		/*
		Updating a GPU resource dynamically from the CPU incurs a performance hit, 
		as the new data must be transferred over from CPU memory to GPU memory.

		That way is good to animated models, but we need to make static buffers for static models, to be more optimal
		*/
		if (SUCCEEDED(DX11Context::GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresource))) 
		{
			memcpy(m_MappedSubresource.pData, data, size);
			DX11Context::GetDeviceContext()->Unmap(m_BufferHandle, NULL);
		}
	}

	void DX11VertexBuffer::Resize(uint32_t size)
	{
		HRESULT hr;
		m_Size = size;
		m_BufferDesc.ByteWidth = m_Size;
		if (m_BufferHandle)
			ReleaseCOM(m_BufferHandle);
		hr = DX11Context::GetContext()->GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &m_BufferHandle);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11VertexBuffer");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11VertexBuffer succesfully created!");
	}

	void DX11VertexBuffer::TranslateLayout()
	{
		HRESULT hr;
		const std::vector<BufferElement>& layout = m_Layout.GetElements();
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[layout.size()];
		for (size_t i = 0; i < layout.size(); i++)
		{
			const BufferElement& element = layout[i];
			desc[i] = { element.Name.c_str(), 0, ShaderDataTypeToDirect3DBaseType(element.Type), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}
		// TODO: Change when we add shaders
		const DX11Shader* shader = DX11Shader::CurrentlyBound();
		BGS_CORE_ASSERT(shader, "Shader doesn't exist");

		hr = DX11Context::GetDevice()->CreateInputLayout(desc, layout.size(), shader->GetData().vs->GetBufferPointer(), shader->GetData().vs->GetBufferSize(), &m_InputLayout);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11InputLayout");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11InputLayout succesfully created!");

		delete[] desc;
	}

	DX11IndexBuffer::DX11IndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		D3D11_BUFFER_DESC ibd;
		ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint32_t);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA ibInitData;
		ibInitData.pSysMem = indices;
		HRESULT hr = DX11Context::GetDevice()->CreateBuffer(&ibd, &ibInitData, &m_BufferHandle);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11IndexBuffer");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11IndexBuffer succesfully created!");
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		ReleaseCOM(m_BufferHandle);
		BGS_CORE_TRACE("D3D11IndexBuffer succesfully released!");
	}

	void DX11IndexBuffer::Bind() const
	{
		DX11Context::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11Context::GetDeviceContext()->IASetIndexBuffer(m_BufferHandle, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind() const
	{
	}

	DX11ConstantBuffer::DX11ConstantBuffer(uint32_t size)
		:m_Size(size)
	{
		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		m_BufferDesc.ByteWidth = size;
		m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		m_BufferDesc.MiscFlags = 0;

		m_BufferHandle = nullptr;

		ReleaseCOM(m_BufferHandle);

		HRESULT hr = DX11Context::GetDevice()->CreateBuffer(&m_BufferDesc, NULL, &m_BufferHandle);
		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11ConstantBuffer");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11ConstantBuffer succesfully created!");
	}

	DX11ConstantBuffer::~DX11ConstantBuffer()
	{
		ReleaseCOM(m_BufferHandle);
		BGS_CORE_TRACE("D3D11ConstantBuffer succesfully released!");
	}

	void DX11ConstantBuffer::SetData(const void* data, uint32_t size)
	{		
		//Direct3DContext::GetDeviceContext()->UpdateSubresource(m_BufferHandle, NULL, NULL, data, NULL, NULL);
		if (SUCCEEDED(DX11Context::GetDeviceContext()->Map(m_BufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_MappedSubresource)))
		{
			memcpy(m_MappedSubresource.pData, data, size);
			DX11Context::GetDeviceContext()->Unmap(m_BufferHandle, NULL);
		}
	}

	void DX11ConstantBuffer::Bind(uint32_t slot) const
	{
		
		DX11Context::GetDeviceContext()->PSSetConstantBuffers(slot, 1, &m_BufferHandle);
		DX11Context::GetDeviceContext()->VSSetConstantBuffers(slot, 1, &m_BufferHandle);

	}

	void DX11ConstantBuffer::Unbind() const
	{
	}
}