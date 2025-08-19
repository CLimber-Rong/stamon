/*
	Name: BinaryWriter.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERR(position) \
	THROW(STMInfo("BinaryWriter", "FileError", \
			String("an error has occured in ") + String(position), \
			"BinaryWriter()"));

#include "Exception.hpp"
#include "String.hpp"
#include "stdio.h"

class BinaryWriter {
public:
	FILE *fstream;
	STMException *ex;

	BinaryWriter() {
	}
    
	BinaryWriter(STMException *e, String filename) {
		ex = e;
		fstream = fopen(filename.getstr(), "wb");
		if (fstream == NULL) {
			FILE_ERR("fopen()");
			return;
		}
	}

	void write(char b) {
		if (!fwrite(&b, 1, 1, fstream)) {
			FILE_ERR("fwrite()");
			return;
		}
    }

	void close() {
		fclose(fstream);
	}
};

#undef FILE_ERR