
project "SupercellTextureLoader"
    kind "StaticLib"

    language "C++"
    cppdialect "C++17"

    targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
    objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

    files {
		"include/**.h",
		"src/**.cpp",
		"src/**.h"
    }

    includedirs {
        "src",
		"include",
        "ThirdParty/Bytestream",
        "ThirdParty/libktx/include",
        "ThirdParty/astc-encoder/include",
        "ThirdParty/ETCPACK/src"
    }

    links {
        -- Compression
        "ETCPACK",
        "ASTC",

        -- KTX
        "ThirdParty/libktx/lib/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}/ktx",
        --"ThirdParty/libktx/lib/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}/ktx_read",
        "ThirdParty/libktx/lib/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}/ObjUtil"

    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"

