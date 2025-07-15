/*
	Name: LineReader.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 03/02/24 16:07
	Description: 行阅读器
*/

#pragma once

#include"Exception.hpp"
#include"String.hpp"
#include"ArrayList.hpp"

ArrayList<String> ImportPaths;

class LineReader {
	public:
		STMException* ex;
		
		LineReader() {}

		LineReader(String filename, STMException* e);

		String getLine();

		bool isMore();

		void close();
};

#undef FILE_ERR