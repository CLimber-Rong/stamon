#ifndef PARSINGQUEUE_CPP
#define PARSINGQUEUE_CPP

#include"FileMap.hpp"
#include"String.hpp"
#include"ArrayList.hpp"

namespace stamon {
	namespace c {
		class ParsingQueue {
			public:
				FileMap map;
				ArrayList<String> file_cache;	//用来存储尚未分析的文件

				ParsingQueue(STMException* e) : map(e) {}

				void AddSource(String src) {
					//新增一个待分析的src
					if(map.exist(src)==true) {
						return;	//已经分析过了
					}
					file_cache.add(src);
				}

				String PopSouce() {
					//弹出一个待分析的源码文件名
					String rst = file_cache[0];
					file_cache.erase(0);
					return rst;
				}
		};
	}
}

#endif