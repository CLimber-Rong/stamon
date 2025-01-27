/*
	Name: Main.cpp
	Copyright: Apache 2.0
	Author: CLimber-Rong
	Date: 24/02/24 11:59
	Description:
    * 命令行工具
    * 该文件并不属于编译器或虚拟机的范畴，所以使用了C标准库
    * 开发者可以自行更改或者建立属于自己的命令行工具
	* 在使用stamon之前，你需要配置环境变量
	* 即：将可执行文件所在目录设为STAMON
*/

#include"Stamon.hpp"

using namespace stamon;

#include"stdio.h"
#include"stdlib.h"
#include"locale.h"

void getHelpInformation();  //输出帮助信息

String getNoEndingSeparatorPath(String path);	//获取末尾没有分隔符的路径

void checkEnvironmentVariableWarning();	//检查环境变量是否存在，不存在则输出警告

enum STAMON_WARNING_SAFE_LEVEL {
	// 警告等级
	StamonWarningSafeLevel_IgnoreWarning = 0,
	StamonWarningSafeLevel_JustWarn,
	StamonWarningSafeLevel_FatalWarning
};

int main(int argc, char* argv[]) {

	//参数表
	ArrayList<String> args;

	//获取可执行文件路径
	String s(argv[0]);

	int warning_level = StamonWarningSafeLevel_JustWarn;	//默认只警告

	for(int i=1; i<argc; i++) {
		args.add(String(argv[i]));
	}

	if(args.empty()) {
		//没有传入任何参数
		printf(
		    "stamon: fatal error: too few arguments\n"
		    "please enter \'stamon help\' to get more information.\n"
		);
		return -1;
	}

	if(
	    args[0].equals(String((char*)"build"))
	    ||args[0].equals(String((char*)"-b"))
	) {

		if(getenv("STAMON")==NULL) {
			printf(
				"stamon: fatal error: missing enviroment variable \"STAMON\"\n"
				"please enter \'stamon help\' to get more information.\n"
			);
			return -1;
		}

		String program_path(getenv("STAMON"));

		String src;

		String dst((char*)"a.stvc");

		bool isSupportImport = true;    //默认支持import
		bool isStrip = false;			//默认附加调试信息

		//解析编译的文件名

		if(args.size()>=2) {
			src = args[1];

			//目标文件名是可选的，默认a.stvc
			
			if(args.size()>=3) {

				dst = args[2];

				for(int i=3; i<args.size(); i++) {

					if(args[i].equals(String((char*)"--import=false"))) {

						isSupportImport = false;

					} else if(args[i].equals(String((char*)"--import=true"))) {

						isSupportImport = true;

					} else if(args[i].equals(String((char*)"--strip=false"))) {

						isStrip = false;

					} else if(args[i].equals(String((char*)"--strip=true"))) {

						isStrip = true;
					
					} else if(args[i].equals(String((char*)"--IgnoreWarning"))) {

						warning_level = StamonWarningSafeLevel_IgnoreWarning;
					
					} else if(args[i].equals(String((char*)"--JustWarn"))) {

						warning_level = StamonWarningSafeLevel_JustWarn;
					
					} else if(args[i].equals(String((char*)"--FatalWarning"))) {

						warning_level = StamonWarningSafeLevel_FatalWarning;
					
					} else if(
					    args[i].length()>=3
					    &&args[i].substring(0, 2).equals((char*)"-I")) {

						//添加引用路径
						ImportPaths.add(
						    getNoEndingSeparatorPath(
						        args[i].substring(2, args[i].length())
						    )
						);

					} else if(
					    args[i].length()>=10
					    &&args[i].substring(0, 9).equals((char*)"--locale=")) {

						//设置语言环境
						setlocale(LC_ALL, args[i].substring(9,args[i].length()).getstr());

					} else {

						//错误参数
						printf(
						    "stamon: compile: bad command\n"
						    "please enter \'stamon help\' "
						    "to get more information.\n"
						);

						return -1;
					}

				}
			}
		} else {
			printf("stamon: build: too few arguments\n"
			       "please enter \'stamon help\' "
			       "to get more information.\n");
			return -1;
		}

		if(isSupportImport) {
			ImportPaths.insert(
			    0,
			    getNoEndingSeparatorPath(program_path)
			    + String((char*)"/include/")
			);
			//加入标准库路径
		}

		Stamon stamon;

		stamon.Init();

		stamon.compile(src, dst, isSupportImport, isStrip);

		if(
			stamon.WarningMsg->empty()==false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning
		) {
			if(warning_level==StamonWarningSafeLevel_JustWarn) {
				printf("stamon: compile: warning:\n");
			} else if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				printf("stamon: compile: fatal error:\n");
			}

			for(int i=0,len=stamon.WarningMsg->size(); i<len; i++) {
				printf("%s\n", stamon.WarningMsg->at(i).getstr());
			}
			if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				return -1;
			}
		}

		if(stamon.ErrorMsg->empty()==false) {
			printf("stamon: compile: fatal error:\n");
			for(int i=0,len=stamon.ErrorMsg->size(); i<len; i++) {
				printf("%s\n", stamon.ErrorMsg->at(i).getstr());
			}
			return -1;
		}

		return 0;

	} else if(
	    args[0].equals(String((char*)"run"))
	    ||args[0].equals(String((char*)"-r"))
	) {

		checkEnvironmentVariableWarning();

		String src;
		bool isGC = true;

		int MemLimit = 16*1024*1024;    //默认虚拟机运行内存16m

		int PoolCacheSize = MemLimit;	//默认内存池缓存大小与运行内存限制一致

		if(args.size()<2) {
			printf("stamon: run: too few arguments\n"
			       "please enter \'stamon help\' "
			       "to get more information.\n");
			return -1;
		} else {
			src = args[1];

			for(int i=2,len=args.size(); i<len; i++) {
				if(args[i].equals(String((char*)"--GC=true"))) {
					isGC = true;
				} else if(args[i].equals(String((char*)"--GC=false"))) {
					isGC = false;
				} else if(
				    args[i].length()>11
				    &&args[i].substring(0, 11).equals(
				        String((char*)"--MemLimit=")
				    )
				) {
					MemLimit = args[i]
					           .substring(11, args[i].length())
					           .toInt();
				} else if(
				    args[i].length()>15
				    &&args[i].substring(0, 15).equals(
				        String((char*)"--MemPoolCache=")
				    )
				) {
					PoolCacheSize = args[i]
									.substring(15, args[i].length())
									.toInt();
				} else if(args[i].equals(String((char*)"--IgnoreWarning"))) {

					warning_level = StamonWarningSafeLevel_IgnoreWarning;
				
				} else if(args[i].equals(String((char*)"--JustWarn"))) {

					warning_level = StamonWarningSafeLevel_JustWarn;
				
				} else if(args[i].equals(String((char*)"--FatalWarning"))) {

					warning_level = StamonWarningSafeLevel_FatalWarning;
				
				} else if(
					args[i].length()>=10
					&&args[i].substring(0, 9).equals((char*)"--locale=")) {

					//设置语言环境
					setlocale(LC_ALL, args[i].substring(9,args[i].length()).getstr());

				} else {
					printf(
					    "stamon: run: bad command\n"
					    "please enter \'stamon help\' "
					    "to get more information.\n"
					);
					return -1;
				}
			}
		}

		Stamon stamon;

		stamon.Init();

		stamon.run(src, isGC, MemLimit, PoolCacheSize);

		if(
			stamon.WarningMsg->empty()==false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning
		) {
			if(warning_level==StamonWarningSafeLevel_JustWarn) {
				printf("stamon: run: warning:\n");
			} else if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				printf("stamon: run: fatal error:\n");
			}
			for(int i=0,len=stamon.WarningMsg->size(); i<len; i++) {
				printf("%s\n", stamon.WarningMsg->at(i).getstr());
			}
			if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				return -1;
			}
		}

		if(stamon.ErrorMsg->empty()==false) {
			printf("stamon: run: fatal error:\n");
			for(int i=0,len=stamon.ErrorMsg->size(); i<len; i++) {
				printf("%s\n", stamon.ErrorMsg->at(i).getstr());
			}
			return -1;
		}

		return 0;

	} else if(
	    args[0].equals(String((char*)"strip"))
	    ||args[0].equals(String((char*)"-s"))
	) {

		checkEnvironmentVariableWarning();

		String src;
		if(args.size()<2) {
			printf("stamon: strip: too few arguments\n"
			       "please enter \'stamon help\' "
			       "to get more information.\n");
			return -1;
		} else {
			src = args[1];
			for(int i=2;i<args.size();i++) {
				if(args[i].equals(String((char*)"--IgnoreWarning"))) {

					warning_level = StamonWarningSafeLevel_IgnoreWarning;
				
				} else if(args[i].equals(String((char*)"--JustWarn"))) {

					warning_level = StamonWarningSafeLevel_JustWarn;
				
				} else if(args[i].equals(String((char*)"--FatalWarning"))) {

					warning_level = StamonWarningSafeLevel_FatalWarning;
				
				}
			}
		}

		Stamon stamon;

		stamon.Init();

		stamon.strip(src);

		if(
			stamon.WarningMsg->empty()==false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning
		) {
			if(warning_level==StamonWarningSafeLevel_JustWarn) {
				printf("stamon: strip: warning:\n");
			} else if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				printf("stamon: strip: fatal error:\n");
			}
			for(int i=0,len=stamon.WarningMsg->size(); i<len; i++) {
				printf("%s\n", stamon.WarningMsg->at(i).getstr());
			}
			if(warning_level==StamonWarningSafeLevel_FatalWarning) {
				return -1;
			}
		}

		if(stamon.ErrorMsg->empty()==false) {
			printf("stamon: strip: fatal error:\n");
			for(int i=0,len=stamon.ErrorMsg->size(); i<len; i++) {
				printf("%s\n", stamon.ErrorMsg->at(i).getstr());
			}
			return -1;
		}

		return 0;

	} else if(
	    args[0].equals(String((char*)"help"))
	    ||args[0].equals(String((char*)"-h"))
	) {
		checkEnvironmentVariableWarning();
		getHelpInformation();
		return 0;
	} else if(
	    args[0].equals(String((char*)"version"))
	    ||args[0].equals(String((char*)"-v"))
	) {
		checkEnvironmentVariableWarning();
		printf(
		    "stamon %d.%d.%d\n"
		    "Be Released by CLimber-Rong(github.com/CLimber-Rong/)\n"
		    "Open Source in \'github.com/CLimber-Rong/stamon/\'\n"
		    "This program has absolutely no warranty.\n",
		    STAMON_VER_X, STAMON_VER_Y, STAMON_VER_Z
		);
		return 0;
	} else {
		checkEnvironmentVariableWarning();
		printf(
		    "stamon: fatal error: bad command\n"
		    "please enter \'stamon help\' "
		    "to get more information.\n"
		);
		return -1;
	}

	return 0;

}

void getHelpInformation() {
	printf(
	    "Usage: stamon options [arguments..]\n"
	    "Options:\n"
	    "\tversion | -v\t\t\tDisplay this version.\n"
	    "\thelp | -h\t\t\tDisplay this information.\n"
	    "\tbuild | -b\t\t\tBuild this source to program.\n"
	    "\t\t<filename>\t\tSource filename (Required)\n"
	    "\t\t<filename>\t\tTarget filename\n"
	    "\t\t--import=<boolean>\t\tSupport import flag\n"
	    "\t\t--strip=<boolean>\t\tStrip debug information flag\n"
	    "\t\t-I<path>\t\tAdd include path\n"
		"\t\t--IgnoreWarning\t\tDo not print warnings\n"
		"\t\t--JustWarn\t\tWarnings are just warnings\n"
		"\t\t--FatalWarning\t\tMake warnings block compilation\n"
		"\t\t--locale=<text>\t\tSet Language Environment\n"
	    "\trun | -r\t\t\tRun STVC.\n"
	    "\t\t<filename>\t\tSource filename (Required)\n"
	    "\t\t--GC=<boolean>\t\tGC flag\n"
	    "\t\t--MemLimit=<Integer>\tSet VM memory limit\n"
		"\t\t--IgnoreWarning\t\tDo not print warnings\n"
		"\t\t--JustWarn\t\tWarnings are just warnings\n"
		"\t\t--FatalWarning\t\tMake warnings block compilation\n"
		"\t\t--locale=<text>\t\tSet Language Environment\n"
	    "\tstrip | -s\t\t\tStrip STVC.\n"
	    "\t\t<filename>\t\tSource filename (Required)\n"
	);
}

String getNoEndingSeparatorPath(String path) {
	//去除末尾分隔符
	if(path[path.length()-1]=='\\' || path[path.length()-1]=='/') {
		return path.substring(0, path.length()-1);
	}
	return path;
}

void checkEnvironmentVariableWarning() {
	if(getenv("STAMON")==NULL) {
		printf(
		    "stamon: warning: missing enviroment variable \"STAMON\"\n"
		    "please enter \'stamon help\' to get more information.\n"
		);
	}
}