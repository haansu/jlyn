workspace "jlyn"
	architecture "x64"

	startproject "jlyn"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "jlyn"
	location "jlyn"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files {
		"jlyn/src/**.h",
		"jlyn/src/**.cpp",
		"jlyn/src/**.hpp"
	}

	includedirs {
		"jlyn/src",
		"jlyn/vendor/SFML/include",
		"jlyn/vendor/spdlog/include/"
	}

	libdirs {
		"jlyn/vendor/SFML/lib"
	}

	links {
		"sfml-system",
		"sfml-window",
		"sfml-graphics",
		"sfml-network",
		"sfml-audio"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			
		}
	
	filter "configurations:Debug"
		defines "JLYN_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "JLYN_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "JLYN_DIST"
		buildoptions "/MD"
		optimize "On"