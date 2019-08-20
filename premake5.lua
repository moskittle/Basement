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


include "Basement/vendor/GLFW"
include "Basement/vendor/Glad"
include "Basement/vendor/ImGui"

project "Basement"
	location "Basement"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-interm/" .. outputdir .. "/%{prj.name}")

	pchheader "bmpch.h"
	pchsource "Basement/source/bmpch.cpp"

	files
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/source",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
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
			"BM_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Backyard")
		}

	filter { "configurations:Debug" }
		defines "BM_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		buildoptions "/MD"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		buildoptions "/MD"
		optimize "On"

project "Backyard"
	location "Backyard"
	kind "ConsoleApp"
	language "C++"

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
		"Basement/source"
	}

	links
	{
		"Basement"
	}

	filter { "system:windows" }
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS"
		}

	filter { "configurations:Debug" }
		defines "BM_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		buildoptions "/MD"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		buildoptions "/MD"
		optimize "On"