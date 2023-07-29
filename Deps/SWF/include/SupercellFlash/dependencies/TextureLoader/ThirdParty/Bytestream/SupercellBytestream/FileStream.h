#pragma once

#include "SupercellBytestream/base/Bytestream.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

using std::ifstream;
using std::ofstream;

namespace sc {
	class WriteFileStream: public Bytestream {
	private:
		ofstream file;
		uint32_t fileSize = 0;

	public:
		WriteFileStream(fs::path filepath) {
			file.open(filepath, std::ios_base::binary);

			file.seekp(0, std::ios::end);
			fileSize = static_cast<uint32_t>(file.tellp());
			file.seekp(0);
		}

		size_t _read(void* data, size_t dataSize) override
		{
			return 0;
		}

		size_t _write(const void* buff, size_t buffSize)
		{
			size_t pos = file.tellp();

			file.write((char*)buff, buffSize);

			const size_t result = (size_t)file.tellp() - pos;
			fileSize += static_cast<uint32_t>(result);
			return result;
		}

		uint32_t tell()
		{
			return static_cast<uint32_t>(file.tellp());
		}

		void seek(uint32_t position)
		{
			file.seekp(position);
		}

		uint32_t size()
		{
			return fileSize;
		}

		bool operator!() {
			return file.fail() || closed;
		}

		void close() {
			file.close();
			closed = true;
		}
	};

	class ReadFileStream : public Bytestream {
	private:
		ifstream file;
		uint32_t fileSize = 0;

	public:
		ReadFileStream(fs::path filepath) {
			file.open(filepath.c_str(), std::ios_base::binary);

			file.seekg(0, std::ios::end);
			fileSize = static_cast<uint32_t>(file.tellg());
			file.seekg(0);
		}

		size_t _read(void* buff, size_t buffSize)
		{
			size_t toRead = (tell() + buffSize) > size() ? size() - tell() : buffSize;

			file.read((char*)buff, toRead);

			return toRead;
		};

		size_t _write(const void* data, size_t dataSize) {
			return 0;
		}

		uint32_t tell()
		{
			return static_cast<uint32_t>(file.tellg());
		}

		void seek(uint32_t position)
		{
			file.seekg(position);
		}

		uint32_t size()
		{
			return fileSize;
		}

		bool operator!() {
			return file.fail() || closed;
		}

		void close() {
			file.close();
			closed = true;
		}
	};
}