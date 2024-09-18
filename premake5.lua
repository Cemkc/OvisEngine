worksapce "Ovis"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Ovis"
	location "Ovis"
	kind "SahredLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h"
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"OV_PLATFORM_WINDOWS",
			"OV_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} .. /bin/" .. outputdir .. "/Sandbox")
		}

	filter "Configurations:Debug"
		define "OV_RELEASE"
		optimize "On"

	filter "configurations:Release"
		define "OV_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		define "OV_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h"
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Ovis/vendor/spdlog/include",
		"Ovis/src"
	}

	links
	{
		"Ovis"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"OV_PLATFORM_WINDOWS",
		}

	filter "Configurations:Debug"
		define "OV_RELEASE"
		optimize "On"

	filter "configurations:Release"
		define "OV_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		define "OV_DIST"
		optimize "On"