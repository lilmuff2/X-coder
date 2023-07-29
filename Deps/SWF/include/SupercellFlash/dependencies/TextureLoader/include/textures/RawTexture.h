#pragma once

#include "TextureLoaderInfo.h"
#include <cstdint>
#include <vector>

namespace sc {
	namespace TextureLoader {
		class RawTexture {
		public:
			ErrorCode SetData(
				uint16_t width, uint16_t height,
				TextureComponents type, TextureElementType elementType,
				std::vector<uint8_t>& buffer
			);

			const std::vector<uint8_t>& GetBuffer();

			virtual uint16_t GetWidth();
			virtual uint16_t GetHeight();

			/// <summary>
			/// Get texture components type
			/// </summary>
			/// <returns>Channel count</returns>
			virtual TextureComponents GetComponentsType();

			/// <summary>
			/// Get texture element type
			/// </summary>
			/// <param name="output"></param>
			/// <returns></returns>
			virtual TextureElementType GetElementType();

		private:
			uint16_t m_width = 0;
			uint16_t m_height = 0;

			TextureComponents m_type = TextureComponents::UNKNWON;
			TextureElementType m_elementType = TextureElementType::UNKNOWN;

			std::vector<uint8_t> m_buffer;
		};
	}
}