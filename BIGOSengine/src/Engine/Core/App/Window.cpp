#include "bgspch.h"
#include "Window.h"

#ifdef BGS_PLATFORM_WINDOWS
	#include "Platform/Win32Window.h"
#endif // BGS_PLATFORM_WINDOWS


namespace BIGOS {

	std::map<void*, Window*> Window::s_Handles;

	Window* Window::StartUpWindow(const WindowProps& props)
	{
#ifdef BGS_PLATFORM_WINDOWS
		return new WindowsWindow(props);
#else
		BT_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif

		return nullptr;
	}
	void Window::RegisterWindowClass(void* handle, Window* window)
	{
		s_Handles[handle] = window;
	}
	Window* Window::GetWindowClass(void* handle)
	{
		if (handle == nullptr)
			return s_Handles.begin()->second;

		return s_Handles[handle];
	}
}