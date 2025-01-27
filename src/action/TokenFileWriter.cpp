/*
	Name: TokenFileWriter.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 28/12/24 17:01
	Description: 词法单元编码文件写入器
*/

#pragma once

#include "BinaryWriter.hpp"
#include "Exception.hpp"
#include "Lexer.cpp"
#include "String.hpp"

//为了节约篇幅，定义了一些宏用于简写
//这些宏只用于此文件
#define CE CATCH { return; }

namespace stamon::action {
    class TokenFileWriter {
        
            STMException* ex;
            BinaryWriter writer;
        public:

            TokenFileWriter() {}

            TokenFileWriter(STMException* e, String filename) {
                ex = e;
                writer = BinaryWriter(ex, filename);
                writer.write(0xAB);
                writer.write(0xDC);
            }

            void writeToken(c::Token* tok) {
                int id = tok->type;

                writer.write(id);
                CE;

                //接着特判带有词法单元数据的词法单元
                if(id==c::TokenInt) {
                    //特判整数token
                    writer.write_i( ((c::IntToken*)tok)->val );
                    CE;
                }

                if(id==c::TokenDouble) {
                    //特判浮点token
                    double val = ((c::DoubleToken*)tok)->val;

                    char* ptr = (char*)&val;

                    writer.write_i( *((int*)ptr) );
                    CE;
                    writer.write_i( *((int*)(ptr+4)) );
                    CE;
                }

                if(id==c::TokenString) {
                    String val = ((c::StringToken*)tok)->val;

                    writer.write_i(val.length());
                    CE;

                    for(int index=0;index<val.length();index++) {
                        writer.write(val[index]);
                        CE;
                    }
                }

                if(id==c::TokenIden) {
                    //特判标识符token，其实现与字符串token类似

                    String iden = ((c::IdenToken*)tok)->iden;

                    writer.write_i(iden.length());
                    CE;

                    for(int index=0;index<iden.length();index++) {
                        writer.write(iden[index]);
                        CE;
                    }
                }
            }


            void writeLineTokens(ArrayList<c::Token*> tokens) {
                //写入一行的Tokens

                for(int i=0,len=tokens.size();i<len;i++) {
                    writeToken(tokens[i]);
                    CE;
                }

                writer.write(-1);   //写入EOL
                CE;
                return;
            }

            void close() {

                writer.write(c::TokenEOF);
                CE;

                writer.close();
                return;
            }
    };
} // namespace stamon::action