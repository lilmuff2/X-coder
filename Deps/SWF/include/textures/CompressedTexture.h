#pragma once

#include "TextureLoaderInfo.h"
#include "textures/RawTexture.h"
#include <cstdint>
#include <vector>

namespace sc {
    class Bytestream;
}

namespace sc {
    namespace TextureLoader {
        class CompressedTexture : public RawTexture {
        public:
            virtual CompressedTextureType ClassType() = 0;

            /// <summary>
            /// Decode texture data to raw vector of pixels
            /// </summary>
            /// <param name="output">Object with decompressed data</param>
            /// <returns>Status code</returns>
            virtual ErrorCode Decode(RawTexture& output) = 0;

            /// <summary>
            /// Encode texture to buffer
            /// </summary>
            /// <param name="output">Vector with encoded texture</param>
            /// <returns>Status code</returns>
            virtual ErrorCode Encode(Bytestream* output) = 0;

            /// <summary>
            /// Check if texture is valid
            /// </summary>
            /// <returns>Valid status</returns>
            virtual ErrorCode IsValid() = 0;

            virtual void Destroy() = 0;
        };
    }
}