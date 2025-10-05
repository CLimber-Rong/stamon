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
#include "stdio.h"

namespace stamon::stdc {

class FileWriter {
	FILE *fstream;

public:
	STMException *ex;

	FileWriter(STMException *e, String filename) {
		ex = e;
		fstream = fopen(filename.getstr(), "wb");
		if (fstream == NULL) {
			FILE_ERR("fopen()");
			return;
		}
	}

	void write(byte b) {
		if (!fwrite(&b, 1, 1, fstream)) {
			FILE_ERR("fwrite()");
			return;
		}
	}

	void close() {
		fclose(fstream);
	}
};

} // namespace stamon::stdc

namespace stamon {

using FileWriter = IFileWriter<stdc::FileWriter>;

}

#undef FILE_ERR