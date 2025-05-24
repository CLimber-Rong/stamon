/*
	Name: BinaryWriter.hpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#define FILE_ERROR { THROW("file opening error") return; }

#include"Exception.hpp"
#include"String.hpp"
#include"stdio.h"

class BinaryWriter {
    public:
        FILE* fstream;
        STMException* ex;

        BinaryWriter() {}
        BinaryWriter(STMException* e, String filename) {
            ex = e;
            fstream = fopen(filename.getstr(), "wb");
            if(fstream==NULL) FILE_ERROR
        }

        void write(char b) {
            if(!fwrite(&b, 1, 1, fstream)) FILE_ERROR
        }

        void write_i(int n) {
            //按大端写入
            for(int i=3;i>=0;i--) {
                write( (n>>(i*8)) & 0xff);
                CATCH {
                    return;
                }
            }
        }

        void close() {
            fclose(fstream);
        }
};

#undef FILE_ERROR