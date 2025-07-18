/*
	Name: BinaryReader.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#include"String.hpp"
#include"Exception.hpp"

class BinaryReader {
	public:
		BinaryReader() {}
		BinaryReader(STMException* e, String filename);

		int getsize();

		char* read();

		void close();
};

#undef FILE_ERROR