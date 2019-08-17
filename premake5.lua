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

project "Basement"
	location "Basement"
	kind "SharedLib"
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
		"%{prj.name}/source",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter { "system:windows" }
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS",
			"BM_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Backyard")
		}

	filter { "configurations:Debug" }
		defines "BM_DEBUG"
		symbols "On"
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
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
		symbols "On"
	
	filter { "configurations:Release" }
		defines "BM_RELEASE"
		optimize "On"
	
	filter { "configurations:Dist" }
		defines "BM_DIST"
		optimize "On"