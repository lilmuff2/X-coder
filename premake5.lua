project "Xcoder"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files {
		"src/*"
    }

    includedirs {
        "ThirdParty/SupercellFlash/include",
		"ThirdParty/SupercellFlash/dependencies/Bytestream",
        "ThirdParty/SupercellFlash/dependencies/Compression/include",
		"ThirdParty/nfd/include",
		"ThirdParty/color",
		"ThirdParty/JSON",
		"ThirdParty/OpenCV/include/"
    }

    links {
        "SupercellCompression",
		"LZMA",
		"LZHAM",
		"Zstandard",
		"SupercellFlash",
		"ThirdParty/nfd/nfd"
    }
	filter {"system:windows", "configurations:Debug"}
	links {
		"ThirdParty/OpenCV/lib/%{cfg.architecture}/%{cfg.system}/static/opencv_world470d"
	}
	postbuildcommands {
		'{COPYFILE} "ThirdParty/OpenCV/lib/%{cfg.architecture}/%{cfg.system}/shared/opencv_world470d.dll" "%{cfg.targetdir}" '
	}

	filter {"system:windows", "configurations:Release"}
	links {
		"ThirdParty/OpenCV/lib/%{cfg.architecture}/%{cfg.system}/static/opencv_world470"
	}
	postbuildcommands {
		'{COPYFILE} "ThirdParty/OpenCV/lib/%{cfg.architecture}/%{cfg.system}/shared/opencv_world470.dll" "%{cfg.targetdir}" '
	}
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"