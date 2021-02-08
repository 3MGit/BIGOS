#include "bgspch.h"
#include "Platform/DirectX/Direct3DContext.h"

namespace BIGOS {

	Direct3DContext::Direct3DContext(WindowProps properties, void* deviceContext)
		:m_WindowProperties(properties)
	{
		Init((HWND)deviceContext);
	}

	void Direct3DContext::Init(HWND hWnd)
	{

	}

	void Direct3DContext::Present()
	{
	}

}