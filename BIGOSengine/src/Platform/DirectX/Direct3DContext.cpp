#include "bgspch.h"
#include "Platform/DirectX/Direct3DContext.h"

// compile shaders
#include <d3dcompiler.h>

namespace BIGOS {

	Direct3DContext::Direct3DContext(WindowProps properties, void* deviceContext)
		:m_WindowProperties(properties)
	{
		Init((HWND)deviceContext);
	}

	void Direct3DContext::Init(HWND hWnd)
	{
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1
		};

		// Creating D3D11Device
		HRESULT hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&dev,
			&m_Direct3DFeatureLevel,
			&devcon
		);

		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11Device");
		if(SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11Device succesfully created!");

		hr = E_ABORT;

		// SwapChain descriptor
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = m_WindowProperties.Width;
		scd.BufferDesc.Height = m_WindowProperties.Height;
		
		scd.BufferCount = 1;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = hWnd;
		scd.SampleDesc.Count =  1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = TRUE;

		// Creating SwapChain
		IDXGIDevice* dxgiDevice = 0;
		IDXGIAdapter* dxgiAdapter = 0;
		IDXGIFactory* dxgiFactory = 0;

		dev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
		hr = dxgiFactory->CreateSwapChain(dev, &scd, &swapchain);

		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11SwapChain");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11SwapChain succesfully created!");

		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();

		Resize(m_WindowProperties.Width, m_WindowProperties.Height);
	}

	void Direct3DContext::Shutdown()
	{
		dev->Release();
		devcon->Release();
		swapchain->Release();
	}

	void Direct3DContext::Resize(uint32_t width, uint32_t height)
	{
		swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		ID3D11Texture2D* backBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		dev->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		backBuffer->Release();

		SetViewport(0, 0, width, height);
	}

	void Direct3DContext::Present()
	{
		swapchain->Present(m_WindowProperties.Vsync, NULL);
	}

	void Direct3DContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		devcon->RSSetViewports(1, &m_ScreenViewport);
	}
}