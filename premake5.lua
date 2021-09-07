workspace "Basement"
	architecture "x64"
	startproject "cs560"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"] = "Basement/vendor/spdlog/include"
IncludeDir["GLFW"] = "Basement/vendor/GLFW/include"
IncludeDir["Glad"] = "Basement/vendor/Glad/include"
IncludeDir["ImGui"] = "Basement/vendor/ImGui"
IncludeDir["glm"] = "Basement/vendor/glm"
IncludeDir["stb_image"] = "Basement/vendor/stb_image"
IncludeDir["assimp"] = "Basement/vendor/assimp/include"

group "Dependencies"
	include "Basement/vendor/GLFW"
	include "Basement/vendor/Glad"
	include "Basement/vendor/ImGui"
group ""

project "Basement"
	location "Basement"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-interm/" .. outputdir .. "/%{prj.name}")

	pchheader "bmpch.h"
	pchsource "Basement/source/bmpch.cpp"

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/source",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter { "system:windows" }
		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS",
			"BM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter { "configurations:Debug" }
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		runtime "Release"		
		optimize "on"

project "cs560"
	location "cs560"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-interm/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"Basement/source",
		"Basement/vendor",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"Basement"
	}

	filter { "system:windows" }

		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS"
		}

	filter { "configurations:Debug" }
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "On"
		links
		{
			"Basement/vendor/assimp/bin/Debug/assimp-vc142-mtd.lib"
		}

	filter { "configurations:Release" }
		defines "BM_RELEASE"
		runtime "Release"
		optimize "On"
		links
		{
			"Basement/vendor/assimp/bin/Release/assimp-vc142-mtd.lib"
		}

	filter { "configurations:Dist" }
		defines "BM_DIST"
		runtime "Release"
		optimize "On"
		links
		{
			"Basement/vendor/assimp/bin/Release/assimp-vc142-mtd.lib"
		}