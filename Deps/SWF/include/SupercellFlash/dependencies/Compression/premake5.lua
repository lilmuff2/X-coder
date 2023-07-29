
project "SupercellCompression"
    kind "StaticLib"

    language "C++"
    cppdialect "C++17"

    targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
	
	-- Sources
	files {
		"src/**.cpp"
	}
	
	-- Headers
	files {
		"include/**.h",
		"src/**.h"
	}

    includedirs {
        "include",
		"src",
		"dependencies/Bytestream",
		"dependencies/lzham/include",
		"dependencies/lzma/include",
		"dependencies/zstd/include"
	}
	
	links {
        "LZMA",
		"LZHAM",
		"Zstandard"
    }

    filter "configurations:Debug"
        runtime "Debug"

        defines {
            "DEBUG"
        }

        symbols "on"
    
    filter "configurations:Release"
        runtime "Release"

        defines {
            "NDEBUG"
        }

        optimize "on"