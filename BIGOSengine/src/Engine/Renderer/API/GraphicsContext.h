#pragma once
#include "Engine/Core/App/Window.h"

namespace BIGOS {

	class GraphicsContext
	{
	public:
		static void Create(WindowProps properties, void* windowHandle, void* windowInstance);
		
	protected:
		static GraphicsContext* s_Context;
	};

}
