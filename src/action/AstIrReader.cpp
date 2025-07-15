/*
	Name: AstIrReader.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 09/12/23 17:34
	Description: IR读取器
*/

#pragma once

#include "AstIr.cpp"
#include "AstIrReaderException.cpp"
#include "BinaryReader.hpp"
#include "DataType.hpp"
#include "NumberMap.hpp"
#include "String.hpp"

namespace stamon::action {
class AstIrReader {
	BinaryReader reader;
	char *buffer; // 代码
	int buffer_size; // 代码大小
	int pos; // 设当前需要读取的字节为buffer[pos]
public:
	int VerX, VerY, VerZ; // 这三个变量代表版本为X.Y.Z
	ArrayList<datatype::DataType *> tableConst; // 常量表

	STMException *ex;

	AstIrReader(STMException *e, String filename) {
		ex = e;
		// 传入字节码和字节码大小
		reader = BinaryReader(ex, filename);
		buffer_size = reader.getsize();
		buffer = reader.read();
	}

	bool ReadHeader() {
		// 读取字节码头，读取失败则抛出异常并返回false，否则返回true

		if (buffer_size < 8) {
			// 字节码太少了
			THROW(exception::astirreader::CodeSizeError("ReadHeader()"));
			return false;
		}

		pos = 0; // 初始化

		if (buffer[pos] == (char) 0xAB && buffer[pos + 1] == (char) 0xDB) {
			// 通过魔数查看该字节码是否为STVC
			NextPos(2);
		} else {
			THROW(exception::astirreader::FormatError("ReadHeader()"));
			return false;
		}
		// 版本计算方法：X、Y、Z共占两个字节
		// 其中X占一个字节，Y占一个字节的前四位，Z占一个字节的后四位

		VerX = buffer[pos];
		VerY = buffer[pos + 1];
		VerZ = buffer[pos + 2];

		NextPos(3);
		// 获取常量表
		int tableConstNumber = ReadInt();
		// 常量表项数
		NextPos(4);

		CATCH {
			// 如果上述代码当中出现了异常
			/*
			 * 值得注意的是，由于我在执行了好几步可能报错的代码后才判错
			 * 所以上述当中的有些值可能是错乱的
			 * 不过反正也要return false，所以这些错乱的值无伤大雅
			 */
			return false;
		}

		if (ReadTableConst(tableConstNumber) == false) {
			return false;
		}

		return true;
	}

	bool ReadTableConst(int size) {
		// 读取常量表，size为常量表项数，pos为下一个读取的字节的下标
		while (size != 0) {
			int type = (int) buffer[pos];

			NextPos(1);

			switch (type) {
			case datatype::IntegerTypeID: {
				int value = ReadInt();
				NextPos(4);
				tableConst.add(new datatype::IntegerType(value));
				break;
			}

			case datatype::FloatTypeID: {
				float value;
				char *tmp = (char *) &value;
				tmp[0] = buffer[pos] << 24;
				tmp[1] = buffer[pos + 1] << 16;
				tmp[2] = buffer[pos + 2] << 8;
				tmp[3] = buffer[pos + 3];
				// 通过写入字节的方式给value赋值
				NextPos(4);
				tableConst.add(new datatype::FloatType(value));
				break;
			}

			case datatype::DoubleTypeID: {
				double value;
				char *tmp = (char *) &value;
				for (int i = 0; i < 8; i++) {
					tmp[i] = buffer[pos + i];
					/*
					 * 比如i=0时：
					 * tmp[0] = buffer[pos]<<56
					 * i=1时
					 * tmp[1] = buffer[pos+1]<<48
					 * 以此类推，这样可以通过写入字节的方式给value赋值
					 */
				}
				NextPos(8);
				tableConst.add(new datatype::DoubleType(value));
				break;
			}

			case datatype::NullTypeID: {
				tableConst.add(new datatype::NullType());
				break;
			}

			case datatype::StringTypeID: {
				int slen = ReadInt();
				NextPos(4);

				char *c_arr = new char[slen + 1];
				c_arr[slen] = '\0';

				for (int i = 0; i < slen; i++) {
					c_arr[i] = buffer[pos + i];
				}
				NextPos(slen);
				String s(c_arr);

				delete[] c_arr;

				tableConst.add(new datatype::StringType(s));
				break;
			}

			case ir::IdenConstTypeID: {
				int slen = ReadInt();
				NextPos(4);

				char *c_arr = new char[slen + 1];
				c_arr[slen] = '\0';

				for (int i = 0; i < slen; i++) {
					c_arr[i] = buffer[pos + i];
				}

				NextPos(slen);

				tableConst.add(new ir::IdenConstType(String(c_arr)));

				delete[] c_arr;
				break;
			}

			default: {
				THROW(exception::astirreader::ConstantsError(
						"ReadTableConst()"));
			}
			}

			CATCH {
				return false;
			}

			size--;
		}
		return true;
	}

	ArrayList<ir::AstIr> ReadIR() {
		ArrayList<ir::AstIr> ir;

		int lineNo = -1;

		String filename = String((char *) "");

		while (pos != buffer_size) {
			int type = ReadInt();

			NextPos(4);

			if (type == -2) {
				// 更新行号
				lineNo = ReadInt();

				NextPos(4);

			} else if (type == -3) {
				// 更新文件名
				int slen = ReadInt();

				NextPos(4);

				char *c_arr = new char[slen];

				for (int i = 0; i < slen; i++) {
					c_arr[i] = buffer[pos + i];
				}

				NextPos(slen);

				filename = String(c_arr);

				delete[] c_arr;

			} else {
				// 正常的IR
				ir::AstIr rst;

				rst.data = ReadInt();

				NextPos(4);

				rst.lineNo = lineNo;
				rst.filename = filename;
				rst.type = type;

				ir.add(rst);
			}

			CATCH {
				return ir;
			}
		}

		return ir;
	}

	void NextPos(int x) {
		// 这个函数把pos向后移动x位，如果pos超出了buffer_size，那么抛出异常
		if (pos >= buffer_size) {
			/*
			 * 为了保护程序不会因为越界而退出
			 * 我在pos越界后，抛出异常并把pos调整至非越界范围
			 */
			pos = 0; // 调整至非越界范围
			THROW(exception::astirreader::CodeSizeError("NextPos"));
		} else {
			pos += x;
		}
		return;
	}

	int ReadInt() {
		// 将buffer[pos]到buffer[pos+3]转换成int

		char arr[4] = { buffer[pos + 3], buffer[pos + 2], buffer[pos + 1],
			buffer[pos] };

		int rst = *((int *) arr);

		return rst;
	}

	void close() {
		reader.close();
	}
};
} // namespace stamon::action