#pragma once
#include "Engine/Core/Core.h"


namespace BIGOS {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Test window" , 
					uint32_t width = 1600, 
					uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static std::unique_ptr<Window> StartUpWindow(const WindowProps& props);

		virtual bool Init() = 0;
		virtual void ShutDown() = 0;
	};

}