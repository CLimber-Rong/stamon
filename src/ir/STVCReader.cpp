/*
	Name: IRReader.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR读取器
*/

#ifndef STVCREADER_CPP
#define STVCREADER_CPP

#include"Exception.hpp"
#include"NumberMap.hpp"
#include"DataType.hpp"
#include"String.hpp"

#include"stdio.h"

namespace stamon {
	namespace ir {
		class STVCReader {
				char* stvc;		//代码
				int stvc_size;	//代码大小
				int pos;		//设当前需要读取的字节为stvc[pos]
			public:
				int VerX, VerY, VerZ;   //这三个变量代表版本为X.Y.Z
				NumberMap<datatype::DataType> tableConst;   //常量表

				STVCReader(char* vmcode, int code_size)
					: tableConst(NumberMap<datatype::DataType>()) {
					//传入字节码和字节码大小
					stvc_size = code_size;
					stvc = vmcode;
				}
				bool ReadHeader() {
					//读取字节码头，读取失败则抛出异常并返回false，否则返回true

					if(stvc_size<8) {
						//字节码太少了
						THROW("The code size is too small")
						return false;
					}

					pos = 0;    //初始化

					if(stvc[pos]==(char)0xAB&&stvc[pos+1]==(char)0xDB) {
						//通过魔数查看该字节码是否为STVC
						NextPos(2);
					} else {
						THROW("Not STVC")
						return false;
					}
					//版本计算方法：X、Y、Z共占两个字节
					//其中X占一个字节，Y占一个字节的前四位，Z占一个字节的后四位

					VerX = stvc[pos];
					VerY = stvc[pos+1]>>4;
					VerZ = stvc[pos+1]&0x0f;
					
					NextPos(2);
					//获取常量表
					int tableConstNumber = (stvc[pos]<<24) + (stvc[pos+1]<<16)
					                       + (stvc[pos+2]<<8) + (stvc[pos+3]);
					//常量表项数
					NextPos(4);

					CATCH {
						//如果上述代码当中出现了异常
						/*
						 * 值得注意的是，由于我在执行了好几步可能报错的代码后才判错
						 * 所以上述当中的有些值可能是错乱的
						 * 不过反正也要return false，所以这些错乱的值无伤大雅
						*/
						return false;
					}

					if(ReadTableConst(tableConstNumber)==false) {
						return false;
					}

					return true;
				}

				bool ReadTableConst(int size) {
					//读取常量表，size为常量表项数，pos为下一个读取的字节的下标
					while(size!=0) {
						unsigned int id = (stvc[pos]<<24) + (stvc[pos+1]<<16)
							            + (stvc[pos+2]<<8) + (stvc[pos+3]);
								//常量索引
						int type = (int)stvc[pos+4];
						NextPos(5);
						if(type==datatype::IntegerTypeID) {
							int value = (stvc[pos]<<24) + (stvc[pos+1]<<16)
							            + (stvc[pos+2]<<8) + (stvc[pos+3]);
							NextPos(4);
							tableConst.put(id, new datatype::IntegerType(value));
						} else if(type==datatype::FloatTypeID) {
							float value;
							char* tmp = (char*)&value;
							tmp[0] = stvc[pos]<<24;
							tmp[1] = stvc[pos+1]<<16;
							tmp[2] = stvc[pos+2]<<8;
							tmp[3] = stvc[pos+3];
							//通过写入字节的方式给value赋值
							NextPos(4);
							tableConst.put(id, new datatype::FloatType(value));
						} else if(type==datatype::DoubleTypeID) {
							double value;
							char* tmp = (char*)&value;
							for(int i=0; i<8; i++) {
								tmp[i] = stvc[pos+i]<<(64-i*8-8);
								/*
								 * 比如i=0时：
								 * tmp[0] = stvc[pos]<<56
								 * i=1时
								 * tmp[1] = stvc[pos+1]<<48
								 * 以此类推，这样可以通过写入字节的方式给value赋值
								*/
							}
							NextPos(8);
							tableConst.put(id, new datatype::DoubleType(value));
						} else if(type==datatype::NullTypeID) {
							tableConst.put(id, new datatype::NullType());
						} else if(type==datatype::StringTypeID) {
							int slen = (stvc[pos]<<24) + (stvc[pos+1]<<16)
							           + (stvc[pos+2]<<8) + (stvc[pos+3]);
							NextPos(4);
							
							char* c_arr = new char[slen];
							for(int i=0; i<slen; i++) {
								c_arr[i] = stvc[pos+i];
							}
							NextPos(slen);
							String s(c_arr);

							delete[] c_arr;
							
							tableConst.put(id, new datatype::StringType(s));
						} else {
							THROW("Unknown constants")
						}

						CATCH {
							return false;
						}

						size--;
					}
					return true;
				}

				void NextPos(int x) {
					//这个函数把pos向后移动x位，如果pos超出了stvc_size，那么抛出异常
					if(pos >= stvc_size) {
						/*
						 * 为了保护程序不会因为越界而退出
						 * 我在pos越界后，抛出异常并把pos调整至非越界范围
						 */
						pos = 0;	//调整至非越界范围
						THROW("The code size is too small")
					} else {
						pos += x;
					}
					return;
				}
		};
	}
}

#endif