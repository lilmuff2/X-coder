#pragma once

#include "CompressedTexture.h"
#include "RawTexture.h"

#include <vector>
#include <cstdint>

using namespace sc::TextureLoader;

namespace sc {
	namespace TextureLoader {
	class AstcTexture : public CompressedTexture {
	public:
		static ErrorCode Decode(
			const std::vector<uint8_t>& data, std::vector<uint8_t>& output,
			uint16_t width, uint16_t height,
			ColorSpace colorSpace,
			uint8_t xBlock, uint8_t yBlock
		);

		static ErrorCode Encode(
			const std::vector<uint8_t>& data, std::vector<uint8_t>& output,
			uint16_t width, uint16_t height,
			ColorSpace colorSpace,
			uint8_t xBlock, uint8_t yBlock
		);

	public:
		/// <summary>
		/// Construct from .astc file data
		/// </summary>
		/// <param name="data">.astc file buffer</param>
		/// <param name="space">Color space</param>
		AstcTexture(std::vector<uint8_t> data, ColorSpace space = ColorSpace::Linear);

		/// <summary>
		/// Construct from raw pixel data
		/// </summary>
		/// <param name="width">Image width</param>
		/// <param name="height">Image height</param>
		/// <param name="xBlocks">ASTC block x size</param>
		/// <param name="yBlocks">ASTC block y size</param>
		/// <param name="data">RGBA pixel array</param>
		AstcTexture(
			RawTexture& data,
			uint8_t xBlocks, uint8_t yBlocks,
			ColorSpace space = ColorSpace::Linear
		);

		void Destroy() {
			delete this;
		}

		CompressedTextureType ClassType() {
			return CompressedTextureType::ASTC;
		}

		uint16_t GetWidth();
		uint16_t GetHeight();

		TextureComponents GetComponentsType();

		TextureElementType GetElementType();

		ErrorCode Decode(RawTexture& output);
		ErrorCode Encode(Bytestream* stream);

		ErrorCode IsValid();

	private:
		uint32_t m_width = 0;
		uint32_t m_height = 0;
		uint32_t m_dimensionCount = 0;

		uint8_t m_blockX = 4;
		uint8_t m_blockY = 4;

		std::vector<uint8_t> m_data;
		ColorSpace m_colorSpace = ColorSpace::Linear;
	};
	}
}