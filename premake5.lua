workspace "OvisEngine"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Create a dictionary for include paths realtive to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ovis/vendor/GLFW/include"
IncludeDir["Glad"] = "Ovis/vendor/Glad/include"
IncludeDir["imgui"] = "Ovis/vendor/imgui"
IncludeDir["glm"] = "Ovis/vendor/glm"
IncludeDir["stb_image"] = "Ovis/vendor/stb_image"

-- Include premake5 file in GLFW path into this file
include "Ovis/vendor/GLFW"
include "Ovis/vendor/Glad"
include "Ovis/vendor/imgui"

project "Ovis"
	location "Ovis"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ovpch.h"
	pchsource "Ovis/src/ovpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"OV_PLATFORM_WINDOWS",
			"OV_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines 
		{
			"OV_DEBUG",
			"OV_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OV_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/TileObjects",
		"Ovis/vendor/spdlog/include",
		"Ovis/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Ovis"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OV_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "OV_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "OV_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "OV_DIST"
		runtime "Release"
		optimize "on"