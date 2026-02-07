/*
	Name: FileWriter.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR(position) \
	THROW(log::Info("FileWriter", "FileError", \
			String("an error has occured in ") + String(position), \
			"FileWriter()"));

#include "IFileWriter.hpp"
#include "stdio.h"

namespace stamon::stdc {

class FileWriter {
	FILE *stream;

public:
	log::Exception *ex;

	FileWriter(log::Exception *e, String filename) {
		ex = e;
		stream = fopen(filename.getstr(), "wb");
		if (stream == NULL) {
			FILE_ERR("fopen()");
			return;
		}
	}

	FileWriter(const FileWriter&) = delete;
	FileWriter& operator=(const FileWriter&) = delete;

	FileWriter(FileWriter&& writer) : stream(writer.stream), ex(writer.ex) {
		writer.stream = NULL;
		writer.ex = NULL;
	}

	FileWriter& operator=(FileWriter&& writer) {
		close();
		stream = writer.stream;
		ex = writer.ex;
		writer.stream = NULL;
		writer.ex = NULL;
		return *this;
	}

	void write(byte b) {
		if (!fwrite(&b, 1, 1, stream)) {
			FILE_ERR("fwrite()");
			return;
		}
	}

	void close() {
		if(stream!=NULL) {
			fclose(stream);
			stream = NULL;
		}
	}

	~FileWriter() {
		close();
	}
};

} // namespace stamon::stdc

namespace stamon {

using FileWriter = interface::IFileWriter<stdc::FileWriter>;

}

#undef FILE_ERR