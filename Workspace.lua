workspace "Xcoder"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    include "./"
	group "swf"
		include "ThirdParty/SupercellFlash"
    group "swf/Compression"
		include "ThirdParty/SupercellFlash/dependencies/Compression"
        include "ThirdParty/SupercellFlash/dependencies/Compression/dependencies/lzma"
        include "ThirdParty/SupercellFlash/dependencies/Compression/dependencies/lzham"
        include "ThirdParty/SupercellFlash/dependencies/Compression/dependencies/zstd"
    group "swf/Compression/Image"
		include "ThirdParty/SupercellFlash/dependencies/TextureLoader"
        include "ThirdParty/SupercellFlash/dependencies/TextureLoader/ThirdParty/ETCPACK"
        include "ThirdParty/SupercellFlash/dependencies/TextureLoader/ThirdParty/astc-encoder"