workspace "Basement"
	architecture "x64"
	startproject "Backyard"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Basement/vendor/GLFW/include"
IncludeDir["Glad"] = "Basement/vendor/Glad/include"
IncludeDir["ImGui"] = "Basement/vendor/ImGui"
IncludeDir["glm"] = "Basement/vendor/glm"
IncludeDir["stb_image"] = "Basement/vendor/stb_image"

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
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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

project "Backyard"
	location "Backyard"
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
		"Basement/vendor/spdlog/include",
		"Basement/source",
		"Basement/vendor",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}"
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
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		runtime "Release"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		runtime "Release"
		optimize "On"
		
project "GoofyLand"
	location "GoofyLand"
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
		"Basement/vendor/spdlog/include",
		"Basement/source",
		"Basement/vendor",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}"
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
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		runtime "Release"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		runtime "Release"
		optimize "On"
		