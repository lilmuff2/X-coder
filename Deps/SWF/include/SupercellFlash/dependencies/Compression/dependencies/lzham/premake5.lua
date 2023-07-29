project "LZHAM"
kind "StaticLib"

language "C++"
cppdialect "C++14"

targetdir "%{wks.location}/build/bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"
objdir "%{wks.location}/build/obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/%{prj.name}"

files {
    "include/*"
}

files { 
"src/lzham_lib.cpp",
-- Decompress
"src/lzhamdecomp/lzham_assert.cpp", "src/lzhamdecomp/lzham_assert.h", "src/lzhamdecomp/lzham_checksum.cpp",
"src/lzhamdecomp/lzham_checksum.h", "src/lzhamdecomp/lzham_config.h", "src/lzhamdecomp/lzham_core.h",
"src/lzhamdecomp/lzham_decomp.h", "src/lzhamdecomp/lzham_helpers.h", "src/lzhamdecomp/lzham_huffman_codes.cpp",
"src/lzhamdecomp/lzham_huffman_codes.h", "src/lzhamdecomp/lzham_lzdecompbase.cpp",
"src/lzhamdecomp/lzham_lzdecompbase.h", "src/lzhamdecomp/lzham_lzdecomp.cpp", "src/lzhamdecomp/lzham_math.h",
"src/lzhamdecomp/lzham_mem.cpp", "src/lzhamdecomp/lzham_mem.h", "src/lzhamdecomp/lzham_platform.cpp",
"src/lzhamdecomp/lzham_platform.h", "src/lzhamdecomp/lzham_prefix_coding.cpp", "src/lzhamdecomp/lzham_prefix_coding.h",
"src/lzhamdecomp/lzham_symbol_codec.cpp", "src/lzhamdecomp/lzham_symbol_codec.h", "src/lzhamdecomp/lzham_timer.cpp",
"src/lzhamdecomp/lzham_timer.h", "src/lzhamdecomp/lzham_traits.h", "src/lzhamdecomp/lzham_types.h",
"src/lzhamdecomp/lzham_utils.h", "src/lzhamdecomp/lzham_vector.cpp", "src/lzhamdecomp/lzham_vector.h",
 -- Compress
"src/lzhamcomp/lzham_lzbase.cpp", "src/lzhamcomp/lzham_lzbase.h", "src/lzhamcomp/lzham_lzcomp.cpp",
"src/lzhamcomp/lzham_lzcomp_internal.cpp", "src/lzhamcomp/lzham_lzcomp_internal.h",
"src/lzhamcomp/lzham_lzcomp_state.cpp", "src/lzhamcomp/lzham_match_accel.cpp", "src/lzhamcomp/lzham_match_accel.h",
"src/lzhamcomp/lzham_null_threading.h", "src/lzhamcomp/lzham_pthreads_threading.h", "src/lzhamcomp/lzham_threading.h"}

includedirs {"include", "src/lzhamcomp", "src/lzhamdecomp"}

defines {"_LIB"}

filter "configurations:Debug"
defines {"_DEBUG"}

filter "configurations:Release"
defines {"NDEBUG"}

filter {"system:windows"}
defines {"WIN32"}
files {"src/lzhamcomp/lzham_win32_threading.cpp", "src/lzhamcomp/lzham_win32_threading.h"}

filter {"system: not windows"}
files {"src/lzhamcomp/lzham_pthreads_threading.cpp"}

filter {"toolset:gcc", "configurations:Debug"}
buildoptions {"-g -Wall -Wextra"}

filter {"toolset:gcc", "configurations:Release"}
buildoptions {"-Wall -Wextra -O3 -fomit-frame-pointer -fexpensive-optimizations"}

filter "toolset:gcc"
buildoptions {"-fno-strict-aliasing -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64"}
