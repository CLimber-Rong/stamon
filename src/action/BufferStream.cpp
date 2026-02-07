/*
	Name: BufferStream.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 23/07/25 14:54
	Description: 内存输入输出流的实现，用于行动工具中编解码器的数据读写
*/

#pragma once

#include "ArrayList.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"
#include "BufferStreamException.cpp"
#include "SmartPtr.hpp"
#include "StamonConfig.hpp"
#include "String.hpp"

// 用于简写的宏
#define CE \
	CATCH { \
		return; \
	}

namespace stamon::action {

class BufferInStream {
	// 内存输入流，引用传递
	SmartPtr<byte[]> buffer;
	int buffer_size;
	SmartPtr<int> read_pos;

public:
	log::Exception *ex;

	BufferInStream(log::Exception *e, SmartPtr<byte[]> buff, int bf_sz)
		: ex(e)
		, buffer(buff)
		, buffer_size(bf_sz)
		, read_pos(new int(0)) // 从起始处读
	{
		// 给定内存块，进行读取
	}

	BufferInStream(log::Exception *e, String filename)
		: ex(e)
		, read_pos(new int(0)) // 从起始处读
		, buffer(NULL, stamon::DestroyNothing) {
		// 给定文件名，进行读取
		FileReader reader(ex, filename);
		CE;

		buffer = reader.read();
		CE;

		buffer_size = reader.getSize();
		reader.close();
	}

	void reset() {
		// 重新从起始处读取
		(*read_pos) = 0;
	}

	byte readByte() {
		// 读取一字节
		byte rst;
		if (*read_pos < buffer_size) {
			rst = buffer[*read_pos];
			(*read_pos)++;
		} else {
			printf("pos %u\n", *read_pos);
			rst = 0;
			THROW(exception::bufferstream::DataSizeError("readByte()"));
		}
		return rst;
	}

	void read(byte *data, int size) {
		// 数据是用大端存储的，在读取时应该根据大小端复原到内存中
		if (config::StamonDecodingEndian == config::StamonCodingBigEndian) {
			// 大端读取
			for (int i = 0; i < size; i++) {
				data[i] = readByte();
				CE;
			}
		}
		if (config::StamonDecodingEndian == config::StamonCodingLittleEndian) {
			// 小端读取
			for (int i = size - 1; i >= 0; i--) {
				data[i] = readByte();
				CE;
			}
		}
	}

	template<typename T> void read(T &data) {
		// 类型转换辅助
		read((byte *) &data, sizeof(T));
	}

	template<typename T, int n> void readArray(T (&data)[n]) {
		// 按数组元素读取
		for (int i = 0; i < n; i++) {
			read(data[i]);
		}
	}

	ArrayList<String> readLines() {
		// 将所有数据当作文本按行读取，存入ArrayList，每行字符串结尾带换行符
		String text(buffer.get());

		ArrayList<String> rst;

		int start = 0, end = 0; // 设当前分割的文本为text[start...end]

		while (end < text.length()) {
			if (text[end] == '\n') {
				// 分割一行文本
				rst.add(text.substring(start, end + 1));
				start = end; // 更新start
			}
			end++;
		}

		rst.add(text.substring(start, end)); // 把最后一行文本写入

		return rst;
	}

	int getSize() {
		return buffer_size;
	}

	bool isMore() {
		return *read_pos < buffer_size;
	}
};

class BufferOutStream {
	// 内存输出流，引用传递
	SmartPtr<ArrayList<byte>> buffer;

public:
	log::Exception *ex;

	BufferOutStream(log::Exception *e)
		: ex(e)
		, buffer(new ArrayList<byte>()) {
	}

	void writeByte(byte data) {
		buffer->add(data);
	}

	void write(const byte *data, int size) {
		// 根据端序写入，以保证生成的数据都是大端
		if (config::StamonEncodingEndian == config::StamonCodingBigEndian) {
			for (int i = 0; i < size; i++) {
				writeByte(data[i]);
			}
		}
		if (config::StamonEncodingEndian == config::StamonCodingLittleEndian) {
			for (int i = size - 1; i >= 0; i--) {
				writeByte(data[i]);
			}
		}
	}

	BufferInStream toBufferInStream() {
		// 转换为BufferInStream，用于数据传递
		SmartPtr<byte[]> buff(new byte[buffer->size()]); // 新建内存块
		for (int i = 0; i < buffer->size(); i++) {
			buff[i] = buffer->at(i);
		}
		return BufferInStream(ex, buff, buffer->size());
	}

	template<typename T> void write(const T &data) {
		// 类型转换辅助
		write((byte *) &data, sizeof(T));
	}

	template<typename T, int n> void write(const T (&data)[n]) {
		// 按数组元素写入
		for (int i = 0; i < n; i++) {
			write(data[i]);
		}
	}

	int getSize() {
		return buffer->size();
	}

	void writeFile(String filename) {
		// 在写入完毕后，调用此函数将其全部保存至文件
		FileWriter writer(ex, filename);
		CE;

		for (int i = 0; i < buffer->size(); i++) {
			writer.write(buffer->at(i));
			CE;
		}
		writer.close();
	}
};

} // namespace stamon::action

#undef CE