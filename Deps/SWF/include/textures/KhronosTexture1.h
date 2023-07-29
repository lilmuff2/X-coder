#pragma once

#include "CompressedTexture.h"
#include "RawTexture.h"

#include <string>
#include <vector>
#include <cassert>

struct ktxTexture;

namespace sc {
    namespace TextureLoader {
        enum class glInternalFormat : uint32_t {
            UNKNOWN = 0,

            // RAW pixels
            // RGB/RGBA

            //GL_R3_G3_B2 = 0x2A10,
            //GL_RGB4 = 0x804F,
            //GL_RGB5 = 0x8050,
            //GL_RGB8 = 0x8051,
            //GL_RGB10 = 0x8052,
            //GL_RGB12 = 0x8053,
            //GL_RGB16 = 0x8054,
            //GL_RGBA2 = 0x8055,
            //GL_RGBA4 = 0x8056,
            //GL_RGB5_A1 = 0x8057,
            GL_RGBA8 = 0x8058,
            //GL_RGB10_A2 = 0x8059,
            //GL_RGBA12 = 0x805A,
            //GL_RGBA16 = 0x805B,


            // ASTC

            GL_COMPRESSED_RGBA_ASTC_4x4 = 0x93B0,
            GL_COMPRESSED_RGBA_ASTC_5x5 = 0x93B2,
            GL_COMPRESSED_RGBA_ASTC_6x6 = 0x93B4,
            GL_COMPRESSED_RGBA_ASTC_8x8 = 0x93B7,
        };

        enum class glType : uint32_t {
            COMPRESSED = 0,

            //GL_BYTE = 0x1400,
            GL_UNSIGNED_BYTE = 0x1401,
            //GL_SHORT = 0x1402,
            //GL_UNSIGNED_SHORT = 0x1403,
            //GL_HALF_FLOAT = 0x140B,
            //GL_FLOAT = 0x1406,
            //GL_FIXED = 0x140C,
        };

        enum class glFormat : uint32_t {
            COMPRESSED = 0,

            GL_R = 0x1903,
            GL_RG = 0x8227,
            GL_RGB = 0x1907,
            GL_RGBA = 0x1908,
            GL_SRGB = 0x8C40,
            GL_SRGB_ALPHA = 0x8C42
        };
    }
}