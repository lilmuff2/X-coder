
project "SupercellCompressionCLI"
    kind "ConsoleApp"

    language "C++"
    cppdialect "C++17"

    targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files {
        "main.cpp"
    }

    includedirs {
		"%{wks.location}/dependencies/Bytestream",
		"%{wks.location}/include"
    }
	
	links {
        "SupercellCompression"
    }

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
    
    filter "configurations:Release"
        defines "NDEBUG"
        runtime "Release"
        optimize "on"
