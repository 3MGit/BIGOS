workspace "BIGOS"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["ImGui"] = "%{wks.location}/BIGOSengine/ThirdParty/imgui"
IncludeDir["Glad"] = "%{wks.location}/BIGOSengine/ThirdParty/Glad/include"
IncludeDir["stb_image"] = "%{wks.location}/BIGOSengine/ThirdParty/stb_image"
IncludeDir["spdlog"] = "%{wks.location}/BIGOSengine/ThirdParty/spdlog/include"
IncludeDir["Vulkan"] = "%{wks.location}/BIGOSengine/ThirdParty/Vulkan"

group "Dependencies"
	include "BIGOSengine/ThirdParty/imgui"
	include "BIGOSengine/ThirdParty/Glad"
group ""

project "BIGOSengine"
	location "BIGOSengine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bgspch.h"
	pchsource "BIGOSengine/src/bgspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/ThirdParty/stb_image/**.h",
		"%{prj.name}/ThirdParty/Vulkan/**.h",
		"%{prj.name}/ThirdParty/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Vulkan}"
	}

	links 
	{ 
		"Glad",
		"ImGui",
		"opengl32.lib",
		"d3d11.lib",
		"D3DCompiler.lib"
	}

	filter "files:**.c"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BGS_PLATFORM_WINDOWS",
			"VK_USE_PLATFORM_WIN32_KHR",
			"VK_NO_PROTOTYPES",
			" BUILD_VK"
		}

	filter "configurations:Debug"
		defines "BGS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BGS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BGS_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BIGOSengine/src",
		"BIGOSengine/ThirdParty",
		"BIGOSengine/ThirdParty/spdlog/include"
	}

	links
	{
		"BIGOSengine"
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BGS_PLATFORM_WINDOWS",
			"VK_USE_PLATFORM_WIN32_KHR"
		}

	filter "configurations:Debug"
		defines "BGS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BGS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BGS_DIST"
		runtime "Release"
		optimize "on"
