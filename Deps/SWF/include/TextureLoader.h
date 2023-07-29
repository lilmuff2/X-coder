#pragma once

#include "TextureLoaderInfo.h"
#include "textures/RawTexture.h"
#include "textures/CompressedTexture.h"
#include "textures/AstcTexture.h"
#include "textures/KhronosTexture.h"

#include <filesystem>
namespace fs = std::filesystem;

// Helper functions
namespace sc {
	TextureLoader::CompressedTextureType GetTextureTypeByExtension(std::string extension);

	std::string GetExtensionByTextureType(TextureLoader::CompressedTextureType type);
}

// Main functions
namespace sc {
	TextureLoader::ErrorCode ScTexture_FromFile(const char* const path, TextureLoader::CompressedTexture** texture);

	TextureLoader::ErrorCode ScTexture_FromFile(fs::path path, TextureLoader::CompressedTexture** texture);

	TextureLoader::ErrorCode ScTexture_FromMemory(std::vector<uint8_t>& buffer, TextureLoader::CompressedTextureType type, TextureLoader::CompressedTexture** texture);

	TextureLoader::ErrorCode ScTexture_ToFile(const char* const path, TextureLoader::CompressedTexture** texture);

	TextureLoader::ErrorCode ScTexture_ToFile(fs::path path, TextureLoader::CompressedTexture** texture);

	void ScTexture_Destroy(TextureLoader::CompressedTexture*& texture);
}