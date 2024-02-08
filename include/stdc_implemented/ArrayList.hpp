/*
	Name: ArrayList
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 28/07/23 17:10
	Description: 动态数组
*/

#ifndef ARRAYLIST
#define ARRAYLIST

#include"stdlib.h"

using namespace std;

template <typename T>
class ArrayList {
		T* list;
		int length;
	public:
		/*值得注意的是：正常的ArrayList赋值，实际上是引用传递*/
	
		ArrayList() {
			list = NULL;
			length = 0;
		}			//创建一个空列表

		ArrayList(int size) {
			list = (T*)calloc(size, sizeof(T));
			length = size;
		}	//创建size个元素的列表

		void add(const T& value) {
			list = (T*)realloc(list, sizeof(T)*(length+1));	//重新分配内存
			list[length] = value;
			length++;
		}		//末尾添加值

		void insert(int index, const T& value) {
			list = (T*)realloc(list, sizeof(T)*(length+1));
			for(int i=length;i>index;i--) {
				list[i] = list[i-1];
			}
			list[index] = value;
			length++;
		}	//将value插入到[index]

		void erase(int index) {
			for(int i=index;i<length-1;i++) {
				list[i] = list[i+1];
			}
			list = (T*)realloc(list, sizeof(T)*(length-1));
			length--;
		}	//删除[index]

		T at(int index) const {
			return list[index];
		}		//获取[index]

		bool empty() const {
			return (length==0);
		}	//判断是否为空

		void clear() {
			free(list);
			list = NULL;
			length = 0;
		}	//清除列表

		int size() const {
			return length;
		}	//获得元素个数

		ArrayList<T> clone() {
			//复制一个相同的ArrayList
			//你可以将这个函数用于值传递
			ArrayList<T> rst(length);
			for(int i=0;i<length;i++) {
				rst[i] = list[i];
			}
			return rst;
		}

		ArrayList<T> operator+(ArrayList<T> src) {
			ArrayList<T> rst = clone();
			for(int i=0,len=src.size();i<len;i++) {
				rst.add(src[i]);
			}
			return rst;
		}

		ArrayList<T> operator+=(ArrayList<T> src) {
			return *(this) = *(this) + src;
		}

		T& operator[](int index) {
			return list[index];
		}

		T operator[](int index) const {
			return list[index];
		}


};

#endif
