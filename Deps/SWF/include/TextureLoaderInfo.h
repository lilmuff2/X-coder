#pragma once

#include <cstdint>

namespace sc {
    namespace TextureLoader {
        enum class CompressedTextureType {
            UNKNOWN,
            KTX,
            //PVR,
            ASTC,
            //PMG
        };

        enum class ErrorCode {
            OK,
            INVALID_OPERATION,
            INVALID_FILE,
            INVALID_PATH,
            WRITING_FAILURE,
            INVALID_PARAM,

            // KTX
            KTX_INVALID_CONTENT,
            KTX_UNSUPORTED_FORMAT,
            KTX_UNSUPORTED_TYPE,
            KTX_UNSUPORTED_COMPRESSION,

            // ASTC
            ASTC_INVALID_FILE,
            ASTC_UNSUPORTED_FILE,
            ASTC_CMP_INIT_FAILURE,
            ASTC_CMP_ALLOC_FAILURE,
            ASTC_CMP_DECOMPRESS_FAILURE,
            ASTC_CMP_COMPRESS_FAILURE
        };

        enum class ColorSpace : uint8_t {
            Linear,
            SRGB
        };

        enum class TextureComponents : uint8_t {
            UNKNWON = 0,

            RGBA = 4,
            RGB = 3,
            LA = 2,
            L = 1
        };

        enum class TextureElementType : uint8_t {
            UNKNOWN,

            UNSIGNED_BYTE
        };

        // KTX

        enum class KhronosTextureVersion : uint8_t {
            KTX1,
            KTX2
        };

        enum class KhronosTextureCompressionType : uint8_t {
            NONE,
            ASTC
            //ETC1
        };
    }
}