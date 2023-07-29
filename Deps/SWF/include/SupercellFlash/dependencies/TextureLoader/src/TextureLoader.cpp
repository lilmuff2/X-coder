#include "TextureLoader.h"

#include "SupercellBytestream.h"

#include "textures/KhronosTexture.h"
#include "textures/AstcTexture.h"
#include <cassert>

namespace sc {
	TextureLoader::CompressedTextureType GetTextureTypeByExtension(std::string extension) {
		std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // converting extension to lowercase

		if (extension == ".ktx" || extension == ".ktx2") {
			return CompressedTextureType::KTX;
		}
		else if (extension == ".astc") {
			return CompressedTextureType::ASTC;
		}

		return CompressedTextureType::UNKNOWN;
	}

	std::string GetExtensionByTextureType(TextureLoader::CompressedTextureType type) {
		switch (type)
		{
		case sc::TextureLoader::CompressedTextureType::KTX:
			return ".ktx";
		case sc::TextureLoader::CompressedTextureType::ASTC:
			return ".astc";
		default:
			return "";
		}
	}
}

namespace sc {
	TextureLoader::ErrorCode ScTexture_FromFile(const char* const path, TextureLoader::CompressedTexture** texture) {
		return ScTexture_FromFile(fs::path(path), texture);
	}

	TextureLoader::ErrorCode ScTexture_FromFile(fs::path path, TextureLoader::CompressedTexture** texture) {
		if (!path.has_extension()) return TextureLoader::ErrorCode::INVALID_PATH;

		CompressedTextureType type = GetTextureTypeByExtension(path.extension().string());

		if (type == CompressedTextureType::UNKNOWN) return ErrorCode::INVALID_FILE;

		ReadFileStream file(path);
		if (!file) {
			return TextureLoader::ErrorCode::INVALID_PATH;
		}

		std::vector<uint8_t> fileBuffer(file.size());
		file.read(fileBuffer.data(), fileBuffer.size());
		return ScTexture_FromMemory(fileBuffer, type, texture);
	}

	TextureLoader::ErrorCode ScTexture_FromMemory(std::vector<uint8_t>& buffer, CompressedTextureType type, TextureLoader::CompressedTexture** texture) {
		assert(texture != nullptr);

		switch (type)
		{
		case sc::TextureLoader::CompressedTextureType::KTX:
			*texture = new TextureLoader::KhronosTexture(buffer);
			break;
		case sc::TextureLoader::CompressedTextureType::ASTC:
			break;
		default:
			break;
		}

		if (*texture != nullptr) {
			TextureLoader::ErrorCode result = (*texture)->IsValid();
			if (result != TextureLoader::ErrorCode::OK) {
				(*texture)->Destroy();
				*texture = nullptr;
			}

			return result;
		}
		else {
			return TextureLoader::ErrorCode::INVALID_FILE;
		}
	}

	void ScTexture_Destroy(TextureLoader::CompressedTexture*& texture) {
		if (texture != nullptr) {
			texture->Destroy();
			texture = nullptr;
		}
	}

	TextureLoader::ErrorCode ScTexture_ToFile(const char* const path, TextureLoader::CompressedTexture** texture) {
		return ScTexture_ToFile(fs::path(path), texture);
	}

	TextureLoader::ErrorCode ScTexture_ToFile(fs::path path, TextureLoader::CompressedTexture** texture) {
		if (texture == nullptr || *texture == nullptr) return ErrorCode::INVALID_OPERATION;

		CompressedTextureType type = (*texture)->ClassType();
		
		std::string extension = GetExtensionByTextureType(type);
		if (path.extension() != extension) {
			path += extension;
		}

		WriteFileStream file(path);
		if (!file) return ErrorCode::INVALID_PATH;

		ErrorCode result = (*texture)->Encode(&file);
		if (result != ErrorCode::OK) {
			return result;
		}

		return ErrorCode::OK;
	}
}