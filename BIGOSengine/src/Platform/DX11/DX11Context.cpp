#include "bgspch.h"
#include "Platform/DX11/DX11Context.h"
#include "Platform/DX11/DX11Common.h"

namespace BIGOS {

	DX11Context::DX11Context(WindowProps properties, void* deviceContext)
		:m_WindowProperties(properties)
	{
		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilBuffer = nullptr;

		m_hWnd = (HWND)deviceContext;
		Init();
	}

	void DX11Context::Init()
	{
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1
		};

		uint32_t createDeviceFlags = 0;
#if defined(BGS_DEBUG) 
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Creating D3D11Device
		HRESULT hr = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createDeviceFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&dev,
			&m_Direct3DFeatureLevel,
			&devcon
		);

		BGS_CORE_ASSERT(SUCCEEDED(hr), "Cannot create D3D11Device");
		if (SUCCEEDED(hr))
			BGS_CORE_TRACE("D3D11Device and ImidieteContext succesfully created!");

		hr = dev->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);
		BGS_CORE_ASSERT(m_MSAAQuality > 0, "");
		m_MSAAEnabled = false;

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
		scd.OutputWindow = m_hWnd;

		scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
		scd.Windowed = TRUE;			// FALSE for fullscreen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


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

		ReleaseCOM(dxgiFactory);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiDevice);

		SetViewport(0, 0, m_WindowProperties.Width, m_WindowProperties.Height);
	}

	void DX11Context::Shutdown()
	{
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);
		BGS_CORE_TRACE("D3D11SwapChain's resources succesfully released!");

		ReleaseCOM(swapchain);
		BGS_CORE_TRACE("D3D11SwapChain succesfully released!");

		ReleaseCOM(dev);
		ReleaseCOM(devcon);
		BGS_CORE_TRACE("D3D11Device and ImidieteContext succesfully released!");
	}

	void DX11Context::Present()
	{
		swapchain->Present(m_WindowProperties.Vsync, NULL);
	}

	void DX11Context::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		//TODO: Think how to handle that with framebuffers
		// Creating backbuffer
		ID3D11Texture2D* backBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		dev->CreateRenderTargetView(backBuffer, NULL, &m_RenderTargetView);
		ReleaseCOM(backBuffer);

		// Creating DepthStencil buffer
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = width ? width : 1;
		depthStencilDesc.Height = height ? height : 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		dev->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer);
		dev->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView);

		devcon->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView); // m_DepthStencilView

		m_ScreenViewport.TopLeftX = (float)x;
		m_ScreenViewport.TopLeftY = (float)y;
		m_ScreenViewport.Width = (float)width;
		m_ScreenViewport.Height = (float)height;
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;
		devcon->RSSetViewports(1, &m_ScreenViewport);
	}
}