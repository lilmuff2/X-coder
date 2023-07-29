workspace "TextureLoader"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    include "./"

    group "Image Compression"
        include "./ThirdParty/astc-encoder"
        include "./ThirdParty/ETCPACK"

