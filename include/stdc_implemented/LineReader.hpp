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

#include"stdio.h"
#include"stdlib.h"

#define FILE_ERR(position) { \
		THROW(\
			STMInfo(\
				"BinaryWriter",\
				"FileError",\
				String("an error has occured in ") + String(position),\
				"BinaryWriter()"\
			)\
		);\
	}

ArrayList<String> ImportPaths;

class LineReader {
		int size;
		FILE* stream = NULL;
		ArrayList<String> split_texts;
		//按行分割后的文本
	public:
		STMException* ex;
		
		LineReader() {}

		LineReader(String filename, STMException* e) {
			ex = e;
			char* buffer = NULL;
			for(int i=0,len=ImportPaths.size(); i<len; i++) {

				stream = fopen(
				             (ImportPaths[i]+filename).getstr(), "r"
				         );

				if(stream!=NULL) {
					break;  //成功打开文件
				}
			}

			if(stream==NULL) {
				//尝试从当前目录打开文件
				stream = fopen(filename.getstr(), "r");
			}

			if(stream==NULL) FILE_ERR("fopen");

			if(fseek(stream, 0, SEEK_END)!=0) FILE_ERR("fseek");
			//将文件指针置于底部

			size = ftell(stream);
			//获取文件指针（即文件底部）与文件头部的距离（即文件大小）

			if(size == -1) FILE_ERR("ftell");

			buffer = (char*)calloc(size+1, 1);    //根据文件大小开辟内存

			if(buffer==NULL)    FILE_ERR("calloc");

			if(fseek(stream, 0, SEEK_SET)!=0) FILE_ERR("fseek");

			fread(buffer, 1, size+1, stream);

			String text = String(buffer);

			free(buffer);

			//然后开始逐行分割

			int start = 0, end=0;	//设当前分割的文本为text[start...end]

			while(end<text.length()) {
				if(text[end]=='\n') {
					//分割一行文本
					split_texts.add(text.substring(start, end+1));
					start = end;	//更新start
				}
				end++;
			}

			split_texts.add(text.substring(start, end));	//把最后一行文本写入
		}

		String getLine() {
			//读取一行的文本
			String rst = split_texts[0];
			split_texts.erase(0);
			return rst;
		}

		bool isMore() {
			return !split_texts.empty();
		}

		void close() {
			fclose(stream);
		}
};

#undef FILE_ERR