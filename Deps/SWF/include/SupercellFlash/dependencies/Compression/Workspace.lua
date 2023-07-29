workspace "ScCompression"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    include "./"
    include "./cli"

    group "Compression"
        include "dependencies/lzma"
        include "dependencies/lzham"
        include "dependencies/zstd"

