/*
	Name: BinaryWriter.hpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 22/02/24 21:52
	Description: 二进制写入器
*/

#pragma once

#include"Exception.hpp"
#include"String.hpp"

class BinaryWriter {
    public:

        BinaryWriter() {}
        BinaryWriter(STMException* e, String filename);

        void write(char b);

        void write_i(int n) {
            //按大端写入
            //这个函数一般无需开发者动手实现
            for(int i=3;i>=0;i++) {
                write( (n>>(i*8)) & 0xff);
                CATCH {
                    return;
                }
            }
        }

        void close();
};

#undef FILE_ERROR