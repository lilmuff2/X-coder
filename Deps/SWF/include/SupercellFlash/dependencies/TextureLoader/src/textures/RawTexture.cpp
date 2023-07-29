#include "textures/RawTexture.h"

namespace sc {
	namespace TextureLoader {
		uint16_t RawTexture::GetWidth(){
			return m_width;
		}

		uint16_t RawTexture::GetHeight() {
			return m_height;
		}

		TextureComponents RawTexture::GetComponentsType() {
			return m_type;
		}

		TextureElementType RawTexture::GetElementType() {
			return m_elementType;
		}

		ErrorCode RawTexture::SetData(
			uint16_t width, uint16_t height,
			TextureComponents type, TextureElementType elementType,
			std::vector<uint8_t>& buffer
		) {
			size_t expectedSize = width * height;

			switch (elementType)
			{
			case sc::TextureLoader::TextureElementType::UNSIGNED_BYTE:
				break;
			default:
				return ErrorCode::INVALID_PARAM;
			}

			switch (type)
			{
			case TextureComponents::RGBA:
				expectedSize *= 4;
				break;
			case TextureComponents::RGB:
				expectedSize *= 3;
				break;
			case TextureComponents::LA:
				expectedSize *= 2;
				break;
			case TextureComponents::L:
				break;
			default:
				return ErrorCode::INVALID_PARAM;
			}

			if (expectedSize <= buffer.size()) {
				m_width = width;
				m_height = height;
				m_buffer = std::vector<uint8_t>(buffer.data(), buffer.data() + expectedSize);
				return ErrorCode::OK;
			}
			else {
				return ErrorCode::INVALID_PARAM;
			}
		}

		const std::vector<uint8_t>& RawTexture::GetBuffer() {
			return m_buffer;
		}
	}
}