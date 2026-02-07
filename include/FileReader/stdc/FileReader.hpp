/*
	Name: FileReader.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR(position) \
	THROW(log::Info("FileReader", "FileError", \
			String("an error has occured in ") + String(position), \
			"FileReader()"));

#include "IFileReader.hpp"
#include "stdio.h"

namespace stamon::stdc {

class FileReader {
	FILE *stream;
	int size;

public:
	log::Exception *ex;

	FileReader(log::Exception *e, String filename) {
		ex = e;

		stream = fopen(filename.getstr(), "rb");
		// 尝试从当前目录打开文件

		for (int i = 0, len = ImportPaths.size(); i < len; i++) {
			if (stream != NULL) {
				break; // 成功打开文件
			}

			stream = fopen((ImportPaths[i] + filename).getstr(), "rb");
		}

		if (stream == NULL) {
			FILE_ERR("fopen()");
			return;
		}

		if (fseek(stream, 0, SEEK_END) != 0) {
			FILE_ERR("fseek()");
			return;
		}
		// 将文件指针置于底部

		size = ftell(stream);
		// 获取文件指针（即文件底部）与文件头部的距离（即文件大小）

		if (size == -1) {
			FILE_ERR("ftell()");
			return;
		}

		if (fseek(stream, 0, SEEK_SET) != 0) {
			FILE_ERR("fseek()");
			return;
		}
		// 将文件指针重新置于顶部
	}

	FileReader(const FileReader &reader) = delete;

	FileReader &operator=(const FileReader &reader) = delete;

	FileReader(FileReader &&reader)
		: stream(reader.stream)
		, size(reader.size)
		, ex(reader.ex) {

		reader.stream = NULL;
		reader.size = 0;
		reader.ex = NULL;

	}

	FileReader &operator=(FileReader &&reader) {
		close();
		stream = reader.stream;
		size = reader.size;
		reader.stream = NULL;
		reader.size = 0;
		reader.ex = NULL;
		return *this;
	}

	int getSize() {
		return size;
	}

	SmartPtr<byte[]> read() {
		// 读取数据，会开辟并返回存储文件二进制数据的堆内存
		SmartPtr<byte[]> buffer(new byte[size + 1]); // 根据文件大小开辟内存
		buffer[size] = '\0';

		if (buffer.get() == NULL) {
			FILE_ERR("calloc()");
			return NULL;
		}

		if (fread(buffer.get(), 1, size, stream) != size) {
			FILE_ERR("fread()");
			return NULL;
		}

		return buffer;
	}

	void close() {
		if (stream != NULL) {
			fclose(stream);
			stream = NULL;
		}
	}

	~FileReader() {
		close();
	}
};

} // namespace stamon::stdc

namespace stamon {

using FileReader = interface::IFileReader<stdc::FileReader>;

}

#undef FILE_ERR