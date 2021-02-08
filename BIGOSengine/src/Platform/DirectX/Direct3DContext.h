#pragma once
#include "Engine/Renderer/GraphicsContext.h"

namespace BIGOS {

	class Direct3DContext : public GraphicsContext
	{
	public:
		Direct3DContext(WindowProps properties, void* deviceContext);

		void Present();

		inline static Direct3DContext* GetContext() { return (Direct3DContext*)s_Context; }
	private:
		void Init(HWND hWnd);
	private:
		WindowProps m_WindowProperties;
	};

}
