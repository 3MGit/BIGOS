#pragma once
#include "Engine/Renderer/GraphicsContext.h"

#include <d3d11.h>

namespace BIGOS {

	class Direct3DContext : public GraphicsContext
	{
	public:
		Direct3DContext(WindowProps properties, void* deviceContext);

		void Present();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		void Resize(uint32_t width, uint32_t height);

		inline static Direct3DContext* GetContext() { return (Direct3DContext*)s_Context; }
		inline static ID3D11DeviceContext* GetDeviceContext() { return GetContext()->devcon; }
		inline static ID3D11RenderTargetView* GetBackBuffer() { return GetContext()->m_RenderTargetView; }
		inline static ID3D11Device* GetDevice() { return GetContext()->dev; }
	private:
		void Init(HWND hWnd);
		void Shutdown();
	public:
		IDXGISwapChain* swapchain;
		ID3D11Device* dev;
		ID3D11DeviceContext* devcon;

		ID3D11RenderTargetView* m_RenderTargetView;
	private:
		D3D_FEATURE_LEVEL m_Direct3DFeatureLevel;

		D3D11_VIEWPORT m_ScreenViewport;
	private:
		WindowProps m_WindowProperties;
	};

}
