#pragma once
#include "Engine/Renderer/API/GraphicsContext.h"

#include <d3d11.h>

namespace BIGOS {

	class Direct3DContext : public GraphicsContext
	{
	public:
		Direct3DContext(WindowProps properties, void* deviceContext);

		void Present();
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		inline static Direct3DContext* GetContext() { return (Direct3DContext*)s_Context; }
		inline static ID3D11DeviceContext* GetDeviceContext() { return GetContext()->devcon; }
		inline static ID3D11RenderTargetView* GetRenderTargetView() { return GetContext()->m_RenderTargetView; }
		inline static ID3D11DepthStencilView* GetDepthStencilView() { return GetContext()->m_DepthStencilView; }
		inline static ID3D11Device* GetDevice() { return GetContext()->dev; }

		inline bool IsMSAAEnabled() const { return m_MSAAEnabled; }
		inline uint32_t GetMSAAQuality() const { return m_MSAAQuality; }
	private:
		void Init();
	public:
		void Shutdown();
	public:
		IDXGISwapChain* swapchain;
		ID3D11Device* dev;
		ID3D11DeviceContext* devcon;
	private:
		uint32_t m_MSAAQuality;
		bool m_MSAAEnabled;
		D3D_FEATURE_LEVEL m_Direct3DFeatureLevel;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11Texture2D* m_DepthStencilBuffer;
		D3D11_VIEWPORT m_ScreenViewport;
	private:
		WindowProps m_WindowProperties;
		HWND m_hWnd;

		friend class WindowsWindow;
	};

}
