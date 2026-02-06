/*
	Name: FileWriter.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR(position) \
	THROW(STMInfo("FileWriter", "FileError", \
			String("an error has occured in ") + String(position), \
			"FileWriter()"));

#include "IFileWriter.hpp"
#include "cstdio"
#include "filesystem"
#include "fstream"
#include "string"

namespace stamon::stdcpp {

class FileWriter {
	std::ofstream stream;

public:
	STMException *ex;

	FileWriter(STMException *e, const String &filename)
		: ex(e) {
		stream.open(std::string(filename.getstr()), std::ios::binary);

		if (!stream.is_open()) {
			FILE_ERR("std::ofstream::open()");
			return;
		}
	}

	FileWriter(const FileWriter&) = delete;
	FileWriter& operator=(const FileWriter&) = delete;

	FileWriter(FileWriter&& writer) : stream(stamon::move(writer.stream)), ex(writer.ex) {
		writer.ex = NULL;
	}

	FileWriter& operator=(FileWriter&& writer) {
		stream = stamon::move(writer.stream);
		ex = writer.ex;
		writer.ex = NULL;
		return *this;
	}

	void write(byte b) {
		if (!stream.put((char) b)) {
			FILE_ERR("std::ofstream::put()");
			return;
		}
	}

	void close() {
		if (stream.is_open()) {
			stream.close();
		}
	}

	~FileWriter() {
		close();
	}
};

} // namespace stamon::stdcpp

namespace stamon {

using FileWriter = interface::IFileWriter<stdcpp::FileWriter>;

}

#undef FILE_ERR