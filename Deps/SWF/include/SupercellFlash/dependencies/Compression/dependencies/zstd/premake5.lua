project "Zstandard"
kind "StaticLib"

language "C"

targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

files {"src/**.c"}

includedirs {"include", "include/common", "include/compress", "include/decompress"}

defines {"ZSTD_DISABLE_ASM", "ZSTD_MULTITHREAD"}

filter {"system:windows"}
defines {"__USE_MINGW_ANSI_STDIO"}

filter "configurations:Debug"
defines {"DEBUG"}
runtime "Debug"
symbols "on"

filter "configurations:Release"
defines {"NDEBUG"}
runtime "Release"
optimize "on"

