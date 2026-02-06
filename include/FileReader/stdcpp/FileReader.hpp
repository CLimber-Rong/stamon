/*
	Name: FileReader.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR(position) \
	THROW(STMInfo("FileReader", "FileError", \
			String("an error has occured in ") + String(position), \
			"FileReader()"));

#include "IFileReader.hpp"
#include "cstdio"
#include "filesystem"
#include "fstream"
#include "string"

namespace stamon::stdcpp {

namespace fs = std::filesystem;

class FileReader {
	std::ifstream stream;
	size_t size;

public:
	STMException *ex;

	FileReader(STMException *e, const String &filename)
		: ex(e) {
		// 首先尝试直接打开文件
		stream.open(std::string(filename.getstr()), std::ios::binary);

		// 如果失败，尝试在导入路径中查找
		if (!stream.is_open()) {
			for (int i = 0, len = ImportPaths.size(); i < len; i++) {
				std::string path = std::string(ImportPaths[i].getstr());
				fs::path full_path = path + "/" + filename.getstr();
				stream.open(full_path, std::ios::binary);
				if (stream.is_open()) {
					break;
				}
			}
		}

		if (!stream.is_open()) {
			FILE_ERR("std::ifstream::open()");
			return;
		}

		// 获取文件大小
		stream.seekg(0, std::ios::end);
		size = stream.tellg();
		stream.seekg(0, std::ios::beg);

		if (!stream) {
			FILE_ERR("std::ifstream::tellg()");
			stream.close();
			return;
		}
	}

	FileReader(const FileReader &) = delete;

	FileReader &operator=(const FileReader &) = delete;

	FileReader(FileReader &&reader) : stream(stamon::move(reader.stream)), ex(reader.ex) {
		reader.ex = NULL;
	}

	FileReader &operator=(FileReader &&reader) {
		stream = stamon::move(reader.stream);
		ex = reader.ex;
		reader.ex = NULL;
		return *this;
	}

	int getSize() const {
		return size;
	}

	SmartPtr<byte[]> read() {
		SmartPtr<byte[]> buffer(new byte[size + 1]);

		stream.read((char *) (buffer.get()), size);

		buffer[size] = '\0'; // 添加终止符

		return buffer;
	}

	void close() {
		if (stream.is_open()) {
			stream.close();
		}
	}

	~FileReader() {
		close();
	}
};

} // namespace stamon::stdcpp

namespace stamon {

using FileReader = interface::IFileReader<stdcpp::FileReader>;

}

#undef FILE_ERR