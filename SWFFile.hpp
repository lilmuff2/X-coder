#pragma once

#include <SupercellFlash.h>
#include <filesystem>

#include <SupercellFlash/Tags.h>
#include <SupercellFlash/exception/NegativeTagLengthException.h>
#include <stb/stb.h>

#include "json.hpp"

using json = nlohmann::ordered_json;

namespace sc
{
	class SWFFile : public SupercellSWF
	{
	public:
		SWFFile() {};
        bool isdl = false;
        bool has_zktx = false;
		// If the file is a real texture, only the texture tags are loaded, otherwise the entire file is loaded.
		SWFFile(std::filesystem::path path)
		{
			current_file = path;
			stream.open_file(path);

			// Path Check
			if (path.string().find("_tex") != std::string::npos)
			{
				load_texures_from_binary();

				return;
			}
			// Whole file loading
            if(path.string().find("_dl") != std::string::npos)
			{
                isdl=true;
				// Skip of all count fields
				stream.seek(17);

				// export names skip
				uint16_t exports_count = stream.read_unsigned_short();

				for (uint16_t i = 0; exports_count > i; i++)
				{
					stream.read_unsigned_short();
				}

				for (uint16_t i = 0; exports_count > i; i++)
				{
					uint8_t length = stream.read_unsigned_byte();
					stream.seek(length, sc::Seek::Add);
				}

				load_texures_from_binary();
			}
		}

	public:
		void load_texures_from_binary()
		{
			while (true)
			{
				uint8_t tag = stream.read_unsigned_byte();
				int32_t tag_length = stream.read_int();

				if (tag == TAG_END)
					break;

				if (tag_length < 0)
					throw NegativeTagLengthException(tag, stream.position());

				switch (tag)
				{
                case TAG_TEXTURE_10:
                    has_zktx = true;
				case TAG_TEXTURE:
				case TAG_TEXTURE_2:
				case TAG_TEXTURE_3:
				case TAG_TEXTURE_4:
				case TAG_TEXTURE_5:
				case TAG_TEXTURE_6:
				case TAG_TEXTURE_7:
				case TAG_TEXTURE_8:
				case TAG_TEXTURE_9:
					textures.emplace_back().load(*this, tag, true);
					break;

				default:
					stream.seek(tag_length, Seek::Add);
					break;
				}
			}
		}

		void save_textures_to_folder(std::filesystem::path output_path)
		{
			json texture_infos = json::array();

			for (uint16_t i = 0; textures.size() > i; i++)
			{
				SWFTexture& texture = textures[i];

				// Texture Info
				{
					std::string encoding;
					{
						switch (texture.encoding())
						{
						case SWFTexture::TextureEncoding::KhronosTexture:
							encoding = "khronos";
							break;
						case SWFTexture::TextureEncoding::Raw:
							encoding = "raw";
							break;
						default:
							break;
						}
					}

					std::string pixel_type = "RGBA8";

					switch (texture.pixel_format())
					{
					case SWFTexture::PixelFormat::RGBA4:
						pixel_type = "RGBA4";
						break;
					case SWFTexture::PixelFormat::RGB5_A1:
						pixel_type = "RGB5_A1";
						break;
					case SWFTexture::PixelFormat::RGB565:
						pixel_type = "RGB565";
						break;
					case SWFTexture::PixelFormat::LUMINANCE8_ALPHA8:
						pixel_type = "LUMINANCE8_ALPHA8";
						break;
					case SWFTexture::PixelFormat::LUMINANCE8:
						pixel_type = "LUMINANCE8";
						break;

					case SWFTexture::PixelFormat::RGBA8:
					default:
						break;
					}

					std::string filtering = "LINEAR_NEAREST";

					switch (texture.filtering)
					{
					case SWFTexture::Filter::LINEAR_MIPMAP_NEAREST:
						filtering = "LINEAR_MIPMAP_NEAREST";
						break;
					case SWFTexture::Filter::NEAREST_NEAREST:
						filtering = "NEAREST_NEAREST";
						break;
					case SWFTexture::Filter::LINEAR_NEAREST:
					default:
						break;
					}

					json texture_info = {
						{"Encoding", encoding},
						{"PixelFormat", pixel_type},
						{"Filtering", filtering},
						{"Linear", texture.linear()},
					};

					texture_infos.push_back(texture_info);
				}

				// Texture Image
				std::filesystem::path basename = output_path.stem();
				std::filesystem::path output_image_path = output_path / basename.concat("_").concat(std::to_string(i)).concat(".png");
				OutputFileStream output_image(output_image_path);

				stb::ImageFormat format = stb::ImageFormat::PNG;

				switch (texture.encoding())
				{
				case SWFTexture::TextureEncoding::KhronosTexture:
				{
					RawImage image(
						texture.image()->width(), texture.image()->height(),
						texture.image()->depth()
					);

					sc::MemoryStream image_data(image.data(), image.data_length());
					((sc::KhronosTexture*)(texture.image()))->decompress_data(image_data);

					stb::write_image(image, format, output_image);
				}
				break;

				case SWFTexture::TextureEncoding::Raw:
					texture.linear(true);
					stb::write_image(
						*(sc::RawImage*)(texture.image()),
						format,
						output_image
					);
					break;

				default:
					break;
				}
			}
            json info = {{"Textures",texture_infos},{"IsDL",isdl},{"HasZKTX",has_zktx},{"Compression","Zstandard"}};
            std::string serialized_data = info.dump(4);

			OutputFileStream file_info(output_path / output_path.stem().concat(".json"));
			file_info.write(serialized_data.data(), serialized_data.size());
		}

		void load_textures_from_folder(std::filesystem::path input)
		{
			current_file = input;
			std::filesystem::path basename = input.stem();
			std::filesystem::path texture_info_path = std::filesystem::path(input / basename.concat(".json"));

			// Texture Info Parsing
			json texture_infos = json::array({});

            if (std::filesystem::exists(texture_info_path))
			{
                std::ifstream file(texture_info_path);
                texture_infos = json::parse(file)["Textures"];
			}


			// Texture Images path gather
			SWFVector<std::filesystem::path> texture_images_paths;

			for (auto const& file_descriptor : std::filesystem::directory_iterator(input))
            {
				std::filesystem::path filepath = file_descriptor.path();
				std::filesystem::path file_extension = filepath.extension();

				if (file_extension == ".png")
				{
					texture_images_paths.push_back(filepath);

				}
			}

			//Texture Converting
			for (uint16_t i = 0; texture_images_paths.size() > i; i++)
			{
				// Image Loading
				RawImage* image = nullptr;
				InputFileStream image_file(texture_images_paths[i]);
				stb::load_image(image_file, &image);
				MemoryStream image_data(image->data(), image->data_length());

				// Image Converting
				SWFTexture texture;
				texture.load_from_image(*image);

				if (texture_infos.size() > i)
				{
					json texture_info = texture_infos[i];

					SWFTexture::PixelFormat texture_type = SWFTexture::PixelFormat::RGBA8;

					if (texture_info["PixelFormat"] == "RGBA4")
					{
						texture_type = SWFTexture::PixelFormat::RGBA4;
					}
					else if (texture_info["PixelFormat"] == "RGB5_A1")
					{
						texture_type = SWFTexture::PixelFormat::RGB5_A1;
					}
					else if (texture_info["PixelFormat"] == "RGB565")
					{
						texture_type = SWFTexture::PixelFormat::RGB565;
					}
					else if (texture_info["PixelFormat"] == "LUMINANCE8_ALPHA8")
					{
						texture_type = SWFTexture::PixelFormat::LUMINANCE8_ALPHA8;
					}
					else if (texture_info["PixelFormat"] == "LUMINANCE8")
					{
						texture_type = SWFTexture::PixelFormat::LUMINANCE8;
					}

					if (texture_info["Filtering"] == "LINEAR_NEAREST")
					{
						texture.filtering = SWFTexture::Filter::LINEAR_NEAREST;
					}
					else if (texture_info["Filtering"] == "LINEAR_MIPMAP_NEAREST")
					{
						texture.filtering = SWFTexture::Filter::LINEAR_MIPMAP_NEAREST;
					}
					else if (texture_info["Filtering"] == "NEAREST_NEAREST")
					{
						texture.filtering = SWFTexture::Filter::NEAREST_NEAREST;
					}

					if (texture_info["Encoding"] == "khronos")
					{
						texture.encoding(SWFTexture::TextureEncoding::KhronosTexture);
					}
					else if (texture_info["Encoding"] == "raw")
					{
						texture.encoding(SWFTexture::TextureEncoding::Raw);
						texture.pixel_format(texture_type);
					}
				}

				// Texture Insert
				if (textures.size() <= i)
				{
					textures.push_back(texture);
				}
				else
				{
					textures[i] = texture;
				}
			}
		}
	};
}
