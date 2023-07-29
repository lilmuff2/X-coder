#include "textures/AstcTexture.h"

#include <thread>
#include "astcenc.h"

#include "SupercellBytestream.h"

static constexpr astcenc_swizzle swizzle{ ASTCENC_SWZ_R, ASTCENC_SWZ_G, ASTCENC_SWZ_B, ASTCENC_SWZ_A };

namespace sc {
	namespace TextureLoader {
		ErrorCode AstcTexture::Decode(
			const std::vector<uint8_t>& data,
			std::vector<uint8_t>& output,
			uint16_t width, uint16_t height,
			ColorSpace colorSpace,
			uint8_t x, uint8_t y
		) {
			astcenc_error ec = ASTCENC_SUCCESS;

			astcenc_profile profile;
			switch (colorSpace)
			{
			case ColorSpace::Linear:
				profile = ASTCENC_PRF_LDR;
				break;
			case ColorSpace::SRGB:
				profile = ASTCENC_PRF_LDR_SRGB;
				break;
			}

			astcenc_config config{};
			ec = astcenc_config_init(
				profile,
				x, y, 1,
				ASTCENC_PRE_MEDIUM, ASTCENC_FLG_DECOMPRESS_ONLY,
				&config
			);
			if (ec != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_INIT_FAILURE;

			struct ASTCencStruct {
				astcenc_context* context = nullptr;
				~ASTCencStruct() {
					astcenc_context_free(context);
				}
			} astcenc;
			astcenc_context*& context = astcenc.context;

			unsigned int threadCount = std::thread::hardware_concurrency();
			if (threadCount == 0) threadCount = 1;
			ec = astcenc_context_alloc(&config, threadCount, &context);
			if (ec != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_ALLOC_FAILURE;

			const size_t uncompressedSize = width * height * 4 * sizeof(uint8_t);
			output = std::vector<uint8_t>(uncompressedSize);
			uint8_t* outputPtr = output.data();

			astcenc_image image{};
			image.dim_x = width;
			image.dim_y = height;
			image.dim_z = 1;
			image.data = reinterpret_cast<void**>(&outputPtr);
			image.data_type = ASTCENC_TYPE_U8;

			ec = astcenc_decompress_image(context, data.data(), data.size(), &image, &swizzle, 0);
			if (ec != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_DECOMPRESS_FAILURE;
			astcenc_decompress_reset(context);

			return ErrorCode::OK;
		}

		ErrorCode AstcTexture::Encode(
			const std::vector<uint8_t>& data, std::vector<uint8_t>& output,
			uint16_t width, uint16_t height,
			ColorSpace colorSpace,
			uint8_t x, uint8_t y
		) {
			unsigned int blockCountX = (width + x - 1) / x;
			unsigned int blockCountY = (height + y - 1) / y;

			astcenc_profile profile = ASTCENC_PRF_LDR;
			switch (colorSpace)
			{
			case ColorSpace::Linear:
				profile = ASTCENC_PRF_LDR;
				break;
			case ColorSpace::SRGB:
				profile = ASTCENC_PRF_LDR_SRGB;
				break;
			}

			astcenc_config config;
			config.block_x = x;
			config.block_y = y;
			config.profile = profile;

			astcenc_error status;
			status = astcenc_config_init(profile, x, y, 1, ASTCENC_PRE_MEDIUM, 0, &config);
			if (status != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_INIT_FAILURE;

			unsigned int threadCount = std::thread::hardware_concurrency();
			if (threadCount == 0) threadCount = 1;

			astcenc_context* context;
			status = astcenc_context_alloc(&config, threadCount, &context);
			if (status != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_ALLOC_FAILURE;

			astcenc_image image;
			image.dim_x = width;
			image.dim_y = height;
			image.dim_z = 1;
			image.data_type = ASTCENC_TYPE_U8;
			uint8_t* decompressedData = (uint8_t*)data.data();
			image.data = reinterpret_cast<void**>(&decompressedData);

			// Space needed for 16 bytes of output per compressed block
			size_t compressedDataSize = blockCountX * blockCountY * 16;
			output = std::vector<uint8_t>(compressedDataSize);

			status = astcenc_compress_image(context, &image, &swizzle, output.data(), compressedDataSize, 0);
			if (status != ASTCENC_SUCCESS) return ErrorCode::ASTC_CMP_COMPRESS_FAILURE;

			astcenc_compress_reset(context);

			return ErrorCode::OK;
		}

		AstcTexture::AstcTexture(std::vector<uint8_t> data, ColorSpace space) {
			m_colorSpace = space;
			BufferStream stream(&data);

			uint32_t magic = stream.readUInt32();
			if (magic != 1554098963) return;

			m_blockX = stream.readUInt8();
			m_blockY = stream.readUInt8();
			uint8_t blockZ = stream.readUInt8();
			if (blockZ > 1) return;

			// 24 bit integer wtf
			m_width = stream.readUInt8() +
				(stream.readUInt8() << 8) +
				(stream.readUInt8() << 16);

			m_height = stream.readUInt8() +
				(stream.readUInt8() << 8) +
				(stream.readUInt8() << 16);

			m_dimensionCount = stream.readUInt8() +
				(stream.readUInt8() << 8) +
				(stream.readUInt8() << 16);

			size_t compressedSize = data.size() - stream.tell();
			m_data = std::vector<uint8_t>(compressedSize);
			size_t result = stream.read(m_data.data(), compressedSize);
			if (result != compressedSize) {
				m_data.clear();
			}
		}

		AstcTexture::AstcTexture(
			RawTexture& data,
			uint8_t xBlocks, uint8_t yBlocks,
			ColorSpace space
		) {
			const uint16_t width = data.GetWidth();
			const uint16_t height = data.GetHeight();

			if (width == 0 || height == 0) return;

			const size_t dataSize = width * height * 4 * sizeof(uint8_t);

			const std::vector<uint8_t>& rawData = data.GetBuffer();

			ErrorCode result = AstcTexture::Encode(rawData, m_data, width, height, space, xBlocks, yBlocks);
			if (result != ErrorCode::OK) {
				m_data.clear();
			}
			else {
				m_width = data.GetWidth();
				m_height = data.GetHeight();
			}
		}

		uint16_t AstcTexture::GetWidth() {
			return (uint16_t)m_width;
		}

		uint16_t AstcTexture::GetHeight() {
			return (uint16_t)m_height;
		}

		TextureComponents AstcTexture::GetComponentsType() {
			return TextureComponents::RGBA;
		}

		TextureElementType AstcTexture::GetElementType() {
			return TextureElementType::UNSIGNED_BYTE;
		}

		ErrorCode AstcTexture::Decode(RawTexture& output) {
			std::vector<uint8_t> result;
			AstcTexture::Decode(
				m_data, result,
				m_width, m_height,
				m_colorSpace,
				m_blockX, m_blockY
			);

			return output.SetData(
				m_width, m_height, 
				GetComponentsType(), GetElementType(),
				result
			);
		}

		ErrorCode AstcTexture::Encode(Bytestream* stream) {
			stream->writeUInt32(1554098963);
			stream->writeUInt8(m_blockX);
			stream->writeUInt8(m_blockY);
			stream->writeUInt8(1);

			stream->writeUInt8(m_width & 0xFF);
			stream->writeUInt8((m_width >> 8) & 0xFF);
			stream->writeUInt8((m_width >> 16) & 0xFF);

			stream->writeUInt8(m_height & 0xFF);
			stream->writeUInt8((m_height >> 8) & 0xFF);
			stream->writeUInt8((m_height >> 16) & 0xFF);

			stream->writeUInt8(m_dimensionCount & 0xFF);
			stream->writeUInt8((m_dimensionCount >> 8) & 0xFF);
			stream->writeUInt8((m_dimensionCount >> 16) & 0xFF);

			stream->write(m_data.data(), m_data.size());

			return ErrorCode::OK;
		}

		ErrorCode AstcTexture::IsValid() {
			if (m_dimensionCount != 1) {
				return ErrorCode::ASTC_UNSUPORTED_FILE;
			}

			if (m_width != 0 && m_height != 0 && m_data.size() != 0) {
				return ErrorCode::OK;
			}
			else {
				return ErrorCode::ASTC_INVALID_FILE;
			}
		}
	}
}