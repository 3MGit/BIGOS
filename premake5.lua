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
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

group "Dependencies"
	include "BIGOSengine/ThirdParty/imgui"
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
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"ImGui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BGS_PLATFORM_WINDOWS"
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
		"BIGOSengine/ThirdParty"
	}

	links
	{
		"BIGOSengine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BGS_PLATFORM_WINDOWS"
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
