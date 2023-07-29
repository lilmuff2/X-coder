#pragma once

#include "KhronosTexture1.h"
#include "KhronosTexture2.h"

namespace sc {
    namespace TextureLoader {
        class KhronosTexture : public CompressedTexture {
        public:
            /// <summary>
            /// Construct from KTX file buffer
            /// </summary>
            /// <param name="data">KTX file buffer (KTX1 or KTX2)</param>
            KhronosTexture(std::vector<uint8_t> data);

            /// <summary>
            /// Construct texture from raw data
            /// </summary>
            /// <param name="type">Type of each element</param>
            /// <param name="format">Format of data e.g RGB, RGBA, etc...</param>
            /// <param name="internalFormat"></param>
            /// <param name="baseFormat"></param>
            /// <param name="data"></param>
            KhronosTexture(
                uint16_t width,
                uint16_t height,
                glType type,
                glFormat format,
                glInternalFormat internalFormat,
                glFormat baseFormat,
                std::vector<uint8_t> data
            );

            void Destroy();
            CompressedTextureType ClassType() {
                return CompressedTextureType::KTX;
            }

            uint16_t GetWidth();
            uint16_t GetHeight();

            TextureComponents GetComponentsType();

            TextureElementType GetElementType();

            ErrorCode Decode(RawTexture& output);
            ErrorCode Decode(RawTexture& output, uint32_t level = 0);

            ErrorCode Encode(Bytestream* stream);

            ErrorCode Compress(glInternalFormat format);
            //ErrorCode Compress(VkFormat format);

            ColorSpace GetColorSpace();
            bool IsCompressed();
            KhronosTextureVersion GetVersion();

            ErrorCode IsValid();
        private:
            ErrorCode DecodeASTC(
                uint16_t width, uint16_t height,
                uint8_t* data, size_t dataSize,
                std::vector<uint8_t>& output
            );

        private:
            TextureComponents GetComponentsType_KTX1();
            TextureElementType GetElementType_KTX1();
            ColorSpace GetColorSpace_KTX1();

            static void GetBlocksASTC(glInternalFormat format, uint8_t& x, uint8_t& y, uint8_t& z);

            static KhronosTextureCompressionType GetCompressionType(glInternalFormat format);

        private:
            TextureComponents GetComponentsType_KTX2();
            TextureElementType GetElementType_KTX2();
            ColorSpace GetColorSpace_KTX2();

            static void GetBlocksASTC(VkFormat format, uint8_t& x, uint8_t& y, uint8_t& z);

            static KhronosTextureCompressionType GetCompressionType(VkFormat format);
            

        private:
            ktxTexture* m_texture = nullptr;
        };
    }
}