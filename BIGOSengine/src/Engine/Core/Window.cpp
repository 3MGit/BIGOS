#include "bgspch.h"
#include "Window.h"

#ifdef BGS_PLATFORM_WINDOWS
	#include "Platform/Win32Window.h"
#endif // BGS_PLATFORM_WINDOWS


namespace BIGOS {

	std::unique_ptr<Window> Window::StartUpWindow(const WindowProps& props)
	{
#ifdef BGS_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		BT_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif

		return nullptr;
	}
}