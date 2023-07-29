#include "textures/KhronosTexture.h"

#include "textures/AstcTexture.h"

#define KHRONOS_STATIC
#include <ktx.h>

#include "SupercellBytestream.h"

namespace sc {
	namespace KtxStreamIO {
		KTX_error_code read(ktxStream* str, void* dst, const ktx_size_t count)
		{
			ktx_size_t nread;

			if (!str || !dst)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			if ((nread = buffer->read(dst, count)) != count) {
				if (feof(str->data.file)) {
					return KTX_FILE_UNEXPECTED_EOF;
				}
				else {
					return KTX_FILE_READ_ERROR;
				}
			}
			str->readpos += count;

			return KTX_SUCCESS;
		}

		KTX_error_code skip(ktxStream* str, const ktx_size_t count)
		{
			if (!str)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			if (buffer->tell() + count > buffer->size()) {
				return KTX_FILE_UNEXPECTED_EOF;
			}
			else {
				buffer->skip((uint32_t)count);
			}

			str->readpos += count;

			return KTX_SUCCESS;
		}

		KTX_error_code write(
			ktxStream* str, const void* src,
			const ktx_size_t size,
			const ktx_size_t count)
		{
			if (!str || !src)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			if ((buffer->write(src, count * size) / size) != count) {
				return KTX_FILE_WRITE_ERROR;
			}

			return KTX_SUCCESS;
		}

		KTX_error_code getpos(ktxStream* str, ktx_off_t* pos)
		{
			if (!str || !pos)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			*pos = buffer->tell();

			return KTX_SUCCESS;
		}

		KTX_error_code setpos(ktxStream* str, ktx_off_t pos)
		{
			ktx_size_t fileSize;
			KTX_error_code result;

			if (!str)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			result = str->getsize(str, &fileSize);

			if (result != KTX_SUCCESS) {
				// Device is likely not seekable.
				return result;
			}

			if (pos > (ktx_off_t)fileSize)
				return KTX_INVALID_OPERATION;

			buffer->seek((uint32_t)pos);
			return KTX_SUCCESS;
		}

		KTX_error_code getsize(ktxStream* str, ktx_size_t* size)
		{
			if (!str || !size)
				return KTX_INVALID_VALUE;

			assert(str->type == eStreamTypeCustom);

			BytestreamBase* buffer = reinterpret_cast<BytestreamBase*>(str->data.custom_ptr.address);

			assert(buffer != nullptr);

			*size = (ktx_size_t)buffer->size(); /* See _getpos for why this cast. */

			return KTX_SUCCESS;
		}

		void destruct(ktxStream*)
		{
		}
	}

	namespace TextureLoader {
		KhronosTexture::KhronosTexture(std::vector<uint8_t> buffer) {
			ktxResult result =
				ktxTexture_CreateFromMemory(
					buffer.data(), buffer.size(),
					KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
					&m_texture
				);

			if (result != ktxResult::KTX_SUCCESS) {
				Destroy();
			}
		}

		void KhronosTexture::Destroy() {
			if (m_texture) {
				ktxTexture_Destroy(m_texture);
			}
			delete this;
		}

		KhronosTexture::KhronosTexture(
			uint16_t width,
			uint16_t height,
			glType type,
			glFormat format,
			glInternalFormat internalFormat,
			glFormat baseFormat,
			std::vector<uint8_t> data
		) {
			ktxTextureCreateInfo info{ 0 };
			info.baseWidth = (ktx_uint32_t)width;
			info.baseHeight = (ktx_uint32_t)height;
			info.glInternalformat = (ktx_uint32_t)internalFormat;

			info.generateMipmaps = false;
			info.isArray = false;
			info.numFaces = 1;
			info.numLevels = 1;
			info.numLayers = 1;
			info.numDimensions = 2;
			info.baseDepth = 1;

			ktxTexture1* kTexture = nullptr;

			KTX_error_code result = ktxTexture1_Create(
				&info,
				KTX_TEXTURE_CREATE_ALLOC_STORAGE,
				&kTexture
			);

			if (result != ktx_error_code_e::KTX_SUCCESS ||
				kTexture == nullptr) return;

			size_t imageSize = ktxTexture_GetImageSize((ktxTexture*)kTexture, 0);
			if (imageSize > data.size()) return;

			// TODO
			memcpy(kTexture->pData, data.data(), imageSize);

			m_texture = (ktxTexture*)kTexture;

			kTexture->glType = (ktx_int32_t)type;
			kTexture->glFormat = (ktx_int32_t)format;
			kTexture->glBaseInternalformat = (ktx_int32_t)baseFormat;
		}

		uint16_t KhronosTexture::GetWidth() {
			return (uint16_t)m_texture->baseWidth;
		}

		uint16_t KhronosTexture::GetHeight() {
			return (uint16_t)m_texture->baseHeight;
		}

		TextureComponents KhronosTexture::GetComponentsType() {
			if (m_texture->classId == class_id::ktxTexture1_c) {
				return GetComponentsType_KTX1();
			}
			else {
				return GetComponentsType_KTX2();
			}
		}

		TextureComponents KhronosTexture::GetComponentsType_KTX1() {
			ktxTexture1* kTexture = (ktxTexture1*)m_texture;
			glFormat format =
				kTexture->glFormat == 0 ?
				(glFormat)kTexture->glBaseInternalformat :
				(glFormat)kTexture->glFormat;

			switch (format)
			{
			case sc::TextureLoader::glFormat::GL_R:
				return TextureComponents::L;
			case sc::TextureLoader::glFormat::GL_RG:
				return TextureComponents::LA;
			case sc::TextureLoader::glFormat::GL_SRGB:
			case sc::TextureLoader::glFormat::GL_RGB:
				return TextureComponents::RGB;
			case sc::TextureLoader::glFormat::GL_RGBA:
			case sc::TextureLoader::glFormat::GL_SRGB_ALPHA:
				return TextureComponents::RGBA;
			default:
				return TextureComponents::UNKNWON;
			}
		}

		TextureComponents KhronosTexture::GetComponentsType_KTX2() {
			ktxTexture2* kTexture = (ktxTexture2*)m_texture;
			return static_cast<TextureComponents>(ktxTexture2_GetNumComponents(kTexture));
		}

		TextureElementType KhronosTexture::GetElementType() {
			if (m_texture->classId == class_id::ktxTexture1_c) {
				return GetElementType_KTX1();
			}
			else {
				return GetElementType_KTX2();
			}
		}

		TextureElementType KhronosTexture::GetElementType_KTX1() {
			ktxTexture1* kTexture = (ktxTexture1*)m_texture;
			glType type = (glType)kTexture->glType;

			if (type != glType::COMPRESSED) {
				switch (type)
				{
				case sc::TextureLoader::glType::GL_UNSIGNED_BYTE:
					return TextureElementType::UNSIGNED_BYTE;
				default:
					return TextureElementType::UNKNOWN;
				}
			}
			else {
				switch ((glInternalFormat)kTexture->glInternalformat)
				{
				case sc::TextureLoader::glInternalFormat::GL_COMPRESSED_RGBA_ASTC_4x4:
				case sc::TextureLoader::glInternalFormat::GL_COMPRESSED_RGBA_ASTC_5x5:
				case sc::TextureLoader::glInternalFormat::GL_COMPRESSED_RGBA_ASTC_6x6:
				case sc::TextureLoader::glInternalFormat::GL_COMPRESSED_RGBA_ASTC_8x8:
					return TextureElementType::UNSIGNED_BYTE;
				default:
					return TextureElementType::UNKNOWN;
				}
			}
		}

		TextureElementType KhronosTexture::GetElementType_KTX2() {
			ktxTexture2* kTexture = (ktxTexture2*)m_texture;
			VkFormat format = (VkFormat)kTexture->vkFormat;

			switch (format)
			{
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
			case sc::TextureLoader::VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				return TextureElementType::UNSIGNED_BYTE;
			default:
				return TextureElementType::UNKNOWN;
			}
		}

		ErrorCode KhronosTexture::Decode(RawTexture& output) {
			return Decode(output, 0);
		}

		ErrorCode KhronosTexture::Decode(RawTexture& output, uint32_t level) {
			if (!IsCompressed()) {
				return ErrorCode::INVALID_OPERATION;
			}

			KhronosTextureCompressionType compression;
			if (m_texture->classId == class_id::ktxTexture1_c) {
				compression = GetCompressionType((glInternalFormat)((ktxTexture1*)m_texture)->glInternalformat);
			}
			else {
				compression = GetCompressionType((VkFormat)((ktxTexture2*)m_texture)->vkFormat);
			}

			level = std::clamp(level, 0u, m_texture->numLevels);

			size_t imageSize = ktxTexture_GetImageSize(m_texture, level);
			const uint32_t imageWidth = std::max(1u, m_texture->baseWidth >> level);
			const uint32_t imageHeight = std::max(1u, m_texture->baseHeight >> level);

			size_t imageOffset = 0;
			ktxTexture_GetImageOffset(m_texture, level, 0, 0, &imageOffset);

			uint8_t* data = m_texture->pData;
			std::vector<uint8_t> decompressedBuffer;
			ErrorCode result = ErrorCode::OK;
			switch (compression)
			{
			case KhronosTextureCompressionType::NONE:
				return ErrorCode::INVALID_OPERATION;
			case KhronosTextureCompressionType::ASTC:
				result = DecodeASTC(
					imageWidth, imageHeight,
					data + imageOffset, imageSize,
					decompressedBuffer
				);
				break;
			default:
				return ErrorCode::INVALID_OPERATION;
			}

			if (result != ErrorCode::OK) {
				return result;
			}

			return output.SetData(
				GetWidth(), GetHeight(),
				GetComponentsType(), GetElementType(),
				decompressedBuffer
			);
		}

		ErrorCode KhronosTexture::Encode(Bytestream* streamPtr) {
			ktxStream stream;

			stream.data.custom_ptr.address = streamPtr;
			stream.data.custom_ptr.allocatorAddress = nullptr;
			stream.data.custom_ptr.size = 0;

			stream.type = eStreamTypeCustom;
			stream.closeOnDestruct = false;
			stream.read = KtxStreamIO::read;
			stream.skip = KtxStreamIO::skip;
			stream.write = KtxStreamIO::write;
			stream.getpos = KtxStreamIO::getpos;
			stream.setpos = KtxStreamIO::setpos;
			stream.getsize = KtxStreamIO::getsize;
			stream.destruct = KtxStreamIO::destruct;

			KTX_error_code result = ktxTexture_WriteToStream(m_texture, &stream);
			if (result != KTX_error_code::KTX_SUCCESS) {
				return ErrorCode::WRITING_FAILURE;
			}
			else {
				return ErrorCode::OK;
			}
		}

		ErrorCode KhronosTexture::Compress(glInternalFormat format) {
			if (!m_texture) return ErrorCode::INVALID_FILE;

			if (IsCompressed()) {
				return ErrorCode::INVALID_OPERATION;
			}

			if (GetVersion() != KhronosTextureVersion::KTX1) return ErrorCode::INVALID_OPERATION;

			glInternalFormat srcFormat = (glInternalFormat)((ktxTexture1*)m_texture)->glInternalformat;
			KhronosTextureCompressionType compression = GetCompressionType(format);

			switch (compression)
			{
			case sc::TextureLoader::KhronosTextureCompressionType::ASTC:
				if (srcFormat != glInternalFormat::GL_RGBA8) {
					return ErrorCode::INVALID_OPERATION;
				}
				break;
			default:
				break;
			}

			ktxTextureCreateInfo info{ 0 };
			info.baseWidth = (ktx_uint32_t)GetWidth();
			info.baseHeight = (ktx_uint32_t)GetHeight();
			info.glInternalformat = (ktx_uint32_t)format;
			info.isArray = false;
			info.baseDepth = m_texture->baseDepth;
			info.numDimensions = m_texture->numDimensions;
			info.numFaces = m_texture->numFaces;
			info.numLayers = m_texture->numLayers;
			info.numLevels = m_texture->numLevels;
			info.generateMipmaps = m_texture->generateMipmaps;

			ktxTexture1* kTexture = (ktxTexture1*)m_texture;
			ktxTexture1* kCompressedTexture = nullptr;
			KTX_error_code result = ktxTexture1_Create(
				&info,
				KTX_TEXTURE_CREATE_ALLOC_STORAGE,
				&kCompressedTexture
			);

			kCompressedTexture->glType = (ktx_uint32_t)glType::COMPRESSED;
			kCompressedTexture->glFormat = (ktx_uint32_t)glFormat::COMPRESSED;

			for (uint32_t levelIndex = 0; levelIndex < m_texture->numLevels; ++levelIndex) {
				size_t imageSize = ktxTexture_GetImageSize(m_texture, levelIndex);
				const auto imageWidth = std::max(1u, m_texture->baseWidth >> levelIndex);
				const auto imageHeight = std::max(1u, m_texture->baseHeight >> levelIndex);

				for (uint32_t faceIndex = 0; faceIndex < m_texture->numFaces; ++faceIndex) {
					for (uint32_t layerIndex = 0; layerIndex < m_texture->numLayers; ++layerIndex) {
						ktx_size_t decompressedDataOffset = 0;
						ktxTexture_GetImageOffset(m_texture, levelIndex, faceIndex, layerIndex, &decompressedDataOffset);

						std::vector<uint8_t> buffer(m_texture->pData, m_texture->pData + imageSize);
						std::vector<uint8_t> compressedBuffer;

						switch (compression)
						{
						case sc::TextureLoader::KhronosTextureCompressionType::ASTC:
						{
							uint8_t x, y, z = 0;
							GetBlocksASTC(format, x, y, z);

							ErrorCode result = AstcTexture::Encode(
								buffer, compressedBuffer,
								imageWidth, imageHeight,
								GetColorSpace(), x, y);

							if (result != ErrorCode::OK) {
								return result;
							}
						}
							break;
						default:
							return ErrorCode::INVALID_OPERATION;
						}

						ktxTexture_SetImageFromMemory(
							(ktxTexture*)kCompressedTexture,
							levelIndex, layerIndex, faceIndex,
							compressedBuffer.data(), compressedBuffer.size()
						);
					}
				}
			}

			ktxTexture_Destroy(m_texture);
			m_texture = (ktxTexture*)kCompressedTexture;
			return ErrorCode::OK;
		}

		ColorSpace KhronosTexture::GetColorSpace() {
			if (m_texture->classId == class_id::ktxTexture1_c) {
				return GetColorSpace_KTX1();
			}
			else {
				return GetColorSpace_KTX2();
			}
		}

		ColorSpace KhronosTexture::GetColorSpace_KTX1() {
			ktxTexture1* kTexture = (ktxTexture1*)m_texture;

			glFormat format =
				kTexture->glFormat == 0 ?
				(glFormat)kTexture->glBaseInternalformat :
				(glFormat)kTexture->glFormat;

			switch (format)
			{
			case glFormat::GL_SRGB:
			case glFormat::GL_SRGB_ALPHA:
				return ColorSpace::SRGB;
			default:
				return ColorSpace::Linear;
			}
		}

		ColorSpace KhronosTexture::GetColorSpace_KTX2() {
			ktxTexture2* kTexture = (ktxTexture2*)m_texture;

			switch ((VkFormat)kTexture->vkFormat)
			{
			case VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				return ColorSpace::Linear;
			default:
				return ColorSpace::Linear;
			}
		}

		bool KhronosTexture::IsCompressed() {
			return m_texture->isCompressed;
		}

		KhronosTextureVersion KhronosTexture::GetVersion() {
			switch (m_texture->classId)
			{
			case class_id::ktxTexture1_c:
				return KhronosTextureVersion::KTX1;
			case class_id::ktxTexture2_c:
				return KhronosTextureVersion::KTX2;
			default:
				return KhronosTextureVersion::KTX1;
			}
		}

		ErrorCode KhronosTexture::IsValid() {
			if (m_texture == nullptr) return ErrorCode::KTX_INVALID_CONTENT;
			if (GetWidth() == 0 || GetHeight() == 0) return ErrorCode::KTX_INVALID_CONTENT;

			if (m_texture->classId == class_id::ktxTexture1_c) {
				ktxTexture1* kTexture = (ktxTexture1*)m_texture;

				switch ((glType)kTexture->glType)
				{
				case glType::GL_UNSIGNED_BYTE:
				case glType::COMPRESSED:
					break;
					//case glType::GL_BYTE:
					//case glType::GL_SHORT:
					//case glType::GL_UNSIGNED_SHORT:
					//case glType::GL_HALF_FLOAT:
					//case glType::GL_FLOAT:
					//case glType::GL_FIXED:
				default:
					return ErrorCode::KTX_UNSUPORTED_TYPE;
				}

				switch ((glFormat)kTexture->glFormat)
				{
				case glFormat::GL_RGBA:
				case glFormat::COMPRESSED:
					break;
					//case glFormat::GL_R:
					//case glFormat::GL_RG:
					//case glFormat::GL_RGB:
					//case glFormat::GL_SRGB:
					//case glFormat::GL_SRGB_ALPHA:
				default:
					return ErrorCode::KTX_UNSUPORTED_FORMAT;
				}

				switch ((glInternalFormat)kTexture->glInternalformat)
				{
				case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_4x4: // Supported compressions
				case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_5x5:
				case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_6x6:
				case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_8x8:
					break;
				case glInternalFormat::UNKNOWN:
				default:
					return ErrorCode::KTX_UNSUPORTED_COMPRESSION;
				}

				switch ((glFormat)kTexture->glBaseInternalformat)
				{
				case glFormat::COMPRESSED:
				case glFormat::GL_RGBA:
					break;
					//case glFormat::GL_R:
					//case glFormat::GL_RG:
					//case glFormat::GL_RGB:
					//case glFormat::GL_SRGB:
					//case glFormat::GL_SRGB_ALPHA:
				default:
					return ErrorCode::KTX_UNSUPORTED_FORMAT;
				}
			}
			else {
				ktxTexture2* kTexture = (ktxTexture2*)m_texture;
				VkFormat format = VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK;

				switch ((VkFormat)kTexture->vkFormat)
				{
				case VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
				case VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
					break;
				case VkFormat::VK_FORMAT_MAX_ENUM:
				case VkFormat::VK_FORMAT_UNDEFINED:
				default:
					return ErrorCode::KTX_UNSUPORTED_COMPRESSION;
				}
			}

			return ErrorCode::OK;
		}

		ErrorCode KhronosTexture::DecodeASTC(
			uint16_t width, uint16_t height,
			uint8_t* data, size_t dataSize,
			std::vector<uint8_t>& output
		) {
			uint8_t x, y, z = 0;

			if (m_texture->classId == class_id::ktxTexture1_c) {
				ktxTexture1* kTexture = (ktxTexture1*)m_texture;

				GetBlocksASTC((glInternalFormat)kTexture->glInternalformat, x, y, z);
			}
			else {
				ktxTexture2* kTexture = (ktxTexture2*)m_texture;

				GetBlocksASTC((VkFormat)kTexture->vkFormat, x, y, z);
			}

			if (x == 0 || y == 0) {
				return ErrorCode::INVALID_OPERATION;
			}

			return AstcTexture::Decode(
				std::vector<uint8_t>(data, data + dataSize), output,
				width, height,
				GetColorSpace(),
				x, y
			);
		}

		void KhronosTexture::GetBlocksASTC(glInternalFormat format, uint8_t& x, uint8_t& y, uint8_t& z) {
			switch (format)
			{
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_4x4:
				x = 4; y = 4; z = 1;
				break;
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_5x5:
				x = 5; y = 5; z = 1;
				break;
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_6x6:
				x = 6; y = 6; z = 1;
				break;
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_8x8:
				x = 8; y = 8; z = 1;
				break;
			default:
				break;
			}
		}

		void KhronosTexture::GetBlocksASTC(VkFormat format, uint8_t& x, uint8_t& y, uint8_t& z) {
			switch (format)
			{
			case VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
				x = 4; y = 4; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
				x = 5; y = 4; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
				x = 5; y = 5; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
				x = 6; y = 5; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
				x = 6; y = 6; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
				x = 8; y = 5; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
				x = 8; y = 6; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
				x = 8; y = 8; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
				x = 10; y = 5; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
				x = 10; y = 6; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
				x = 10; y = 8; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
				x = 10; y = 10; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
				x = 12; y = 10; z = 1;
				break;
			case VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				x = 12; y = 12; z = 1;
				break;
			default:
				break;
			}
		}

		KhronosTextureCompressionType KhronosTexture::GetCompressionType(glInternalFormat format) {
			switch (format)
			{
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_4x4: // Supported compressions
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_5x5:
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_6x6:
			case glInternalFormat::GL_COMPRESSED_RGBA_ASTC_8x8:
				return KhronosTextureCompressionType::ASTC;
			default:
				return KhronosTextureCompressionType::NONE;
			}
		}

		KhronosTextureCompressionType KhronosTexture::GetCompressionType(VkFormat format) {
			switch (format)
			{
			case VkFormat::VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
			case VkFormat::VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
				return KhronosTextureCompressionType::ASTC;
			default:
				return KhronosTextureCompressionType::NONE;
			}
		}
	}
}