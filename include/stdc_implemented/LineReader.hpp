/*
	Name: LineReader.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 03/02/24 16:07
	Description: 行阅读器
*/

#ifndef LINEREADER_HPP
#define LINEREADER_HPP

#include"Exception.hpp"
#include"String.hpp"

#include"stdio.h"
#include"stdlib.h"

#define FILE_ERR { THROW("file opening error") return; }
//这个宏用于简写，并且该宏只能在本文件中使用

class LineReader {
        int size;
		FILE* stream;
        char* buffer;
    public:
        STMException* ex;

        LineReader(){}

        LineReader(String filename, STMException* e) {
            ex = e;
            stream = fopen(filename.getstr(), "r");

            if(stream==NULL) FILE_ERR

            if(fseek(stream, 0, SEEK_END)!=0) FILE_ERR
            //将文件指针置于底部

            size = ftell(stream);
            //获取文件指针（即文件底部）与文件头部的距离（即文件大小）

            if(size == -1) FILE_ERR

            buffer = (char*)calloc(1, size+1);    //根据文件大小开辟内存

            if(buffer==NULL)    FILE_ERR

            if(fseek(stream, 0, SEEK_SET)!=0) FILE_ERR
            //将文件指针重新置于顶部
        }
        
        String getLine() {
            char* s = fgets(buffer, size, stream);
            
            if(s==NULL) {
                return String((char*)"\0");
            }

            return String(s);
        }

        bool isMore() {

            int pos = ftell(stream);    //获取当前指针所在位置
            
            bool result = true;

            if(getc(stream)==EOF) {
                result = false;
            }

            if(fseek(stream, pos, SEEK_SET)!=0) {
                THROW("file opening error")
                return false;
            }

            //将文件指针向文件顶部跳动一字符
            return result;
        }

        ~LineReader() {
            free(buffer);
            fclose(stream);
        }

};

#undef FILE_ERR

#endif