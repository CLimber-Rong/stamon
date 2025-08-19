/*
	Name: Main.cpp
	License: Apache 2.0
	Author: CLimber-Rong
	Date: 24/02/24 11:59
	Description:
	* 命令行工具
	* 该文件并不属于编译器或虚拟机的范畴，所以使用了C标准库
	* 开发者可以自行更改或者建立属于自己的命令行工具
	* 在使用stamon之前，你需要配置环境变量
	* 即：将可执行文件所在目录设为STAMON
*/

#include "Stamon.hpp"

using namespace stamon;
using namespace stamon::config; // 由于命名空间前缀过于繁琐，因此引用此命名空间

#include "locale.h"
#include "stdio.h"
#include "stdlib.h"

String getNoEndingSeparatorPath(String path); // 获取末尾没有分隔符的路径
void checkEnvironmentVariableWarning(); // 检查环境变量是否存在，不存在则输出警告
int StamonVersionCommand();
int StamonHelpCommand();
int StamonBuildCommand(ArrayList<String> args);
int StamonRunCommand(ArrayList<String> args);
int StamonStripCommand(ArrayList<String> args);

int main(int argc, char *argv[]) {
	// 参数表
	ArrayList<String> args;

	// 获取可执行文件路径
	String s(argv[0]);

	for (int i = 1; i < argc; i++) {
		args.add(String(argv[i]));
	}

	if (args.empty()) {
		// 没有传入任何参数
		printf("stamon: fatal error: too few arguments\n"
			   "please enter \'stamon help\' to get more information.\n");
		return -1;
	}

	if (args[0].equals(String((char *) "build"))
			|| args[0].equals(String((char *) "-b"))) {
		return StamonBuildCommand(args);

	} else if (args[0].equals(String((char *) "run"))
			   || args[0].equals(String((char *) "-r"))) {
		return StamonRunCommand(args);

	} else if (args[0].equals(String((char *) "strip"))
			   || args[0].equals(String((char *) "-s"))) {
		return StamonStripCommand(args);

	} else if (args[0].equals(String((char *) "help"))
			   || args[0].equals(String((char *) "-h"))) {
		return StamonHelpCommand();

	} else if (args[0].equals(String((char *) "version"))
			   || args[0].equals(String((char *) "-v"))) {
		return StamonVersionCommand();

	} else {
		checkEnvironmentVariableWarning();
		printf("stamon: fatal error: bad command\n"
			   "please enter \'stamon help\' "
			   "to get more information.\n");
		return -1;
	}

	return 0;
}

String getNoEndingSeparatorPath(String path) {
	// 去除末尾分隔符
	if (path[path.length() - 1] == '\\' || path[path.length() - 1] == '/') {
		return path.substring(0, path.length() - 1);
	}
	return path;
}

void checkEnvironmentVariableWarning() {
	if (getenv("STAMON") == NULL) {
		printf("stamon: warning: missing enviroment variable \"STAMON\"\n"
			   "please enter \'stamon help\' to get more information.\n");
	}
}

int StamonVersionCommand() {
	checkEnvironmentVariableWarning();
	printf("stamon %d.%d.%d\n"
		   "Be Released by CLimber-Rong(github.com/CLimber-Rong/)\n"
		   "Open Source on \'https://github.com/CLimber-Rong/stamon/\'\n"
		   "This program has absolutely no warranty.\n",
			STAMON_VER_X, STAMON_VER_Y, STAMON_VER_Z);
	return 0;
}

int StamonHelpCommand() {
	checkEnvironmentVariableWarning();
	printf("Usage: stamon options [arguments..]\n"
		   "Options:\n"
		   "\tversion | -v\t\t\tDisplay this version.\n"
		   "\thelp | -h\t\t\tDisplay this information.\n"
		   "\tbuild | -b\t\t\tBuild this source to program.\n"
		   "\t\t<filename>\t\tSource filename (Required)\n"
		   "\t\t<filename>\t\tTarget filename (Required)\n"
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
		   "\t\t--MemPoolCache=<Integer>\tSet MemoryPool Cache Size\n"
		   "\t\t--IgnoreWarning\t\tDo not print warnings\n"
		   "\t\t--JustWarn\t\tWarnings are just warnings\n"
		   "\t\t--FatalWarning\t\tMake warnings block compilation\n"
		   "\t\t--locale=<text>\t\tSet Language Environment\n"
		   "\tstrip | -s\t\t\tStrip STVC.\n"
		   "\t\t<filename>\t\tSource filename (Required)\n");
	return 0;
}

int StamonBuildCommand(ArrayList<String> args) {
	if (getenv("STAMON") == NULL) {
		printf("stamon: fatal error: missing enviroment variable \"STAMON\"\n"
			   "please enter \'stamon help\' to get more information.\n");
		return -1;
	}

	String program_path(getenv("STAMON"));

	String src;

	// 先设置为配置文件的默认值
	String dst = stamon::c::config::DefaultObjectFileName;
	int warning_level = stamon::c::config::WarningLevel;
	bool isSupportImport = stamon::c::config::isSupportImport;
	bool isStrip = stamon::c::config::isStrip;

	// 解析编译的文件名

	if (args.size() >= 2) {
		src = args[1];

		// 目标文件名是可选的

		if (args.size() >= 3) {
			dst = args[2];

			for (int i = 3; i < args.size(); i++) {
				if (args[i].equals(String((char *) "--import=false"))) {
					isSupportImport = false;

				} else if (args[i].equals(String((char *) "--import=true"))) {
					isSupportImport = true;

				} else if (args[i].equals(String((char *) "--strip=false"))) {
					isStrip = false;

				} else if (args[i].equals(String((char *) "--strip=true"))) {
					isStrip = true;

				} else if (args[i].equals(String((char *) "--IgnoreWarning"))) {
					warning_level = StamonWarningSafeLevel_IgnoreWarning;

				} else if (args[i].equals(String((char *) "--JustWarn"))) {
					warning_level = StamonWarningSafeLevel_JustWarn;

				} else if (args[i].equals(String((char *) "--FatalWarning"))) {
					warning_level = StamonWarningSafeLevel_FatalWarning;

				} else if (args[i].length() >= 3
						   && args[i].substring(0, 2).equals((char *) "-I")) {
					// 添加引用路径
					ImportPaths.add(getNoEndingSeparatorPath(args[i].substring(
											2, args[i].length()))
									+ String("/"));

				} else if (args[i].length() >= 10
						   && args[i].substring(0, 9).equals(
								   (char *) "--locale=")) {
					// 设置语言环境
					setlocale(LC_ALL,
							args[i].substring(9, args[i].length()).getstr());

				} else {
					// 错误参数
					printf("stamon: compile: bad command\n"
						   "please enter \'stamon help\' "
						   "to get more information.\n");

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

	if (isSupportImport) {
		ImportPaths.insert(0, getNoEndingSeparatorPath(program_path)
									  + String((char *) "/include/"));
		// 加入标准库路径
	}

	Stamon stamon;

	stamon.init();

	stamon.compile(src, dst, isSupportImport, isStrip);

	if (stamon.WarningMsg.empty() == false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning) {
		if (warning_level == StamonWarningSafeLevel_JustWarn) {
			printf(
				"stamon: compile: %d warning(s):\n",
				stamon.WarningMsg.size()
			);
		} else if (warning_level
				   == stamon::config::StamonWarningSafeLevel_FatalWarning) {
			printf(
				"stamon: compile: %d fatal warning(a):\n",
				stamon.WarningMsg.size()
			);
		}

		for (int i = 0, len = stamon.WarningMsg.size(); i < len; i++) {
			printf("%s\n", stamon.WarningMsg[i].getstr());
		}
		if (warning_level == StamonWarningSafeLevel_FatalWarning) {
			return -1;
		}
	}

	if (stamon.ErrorMsg.empty() == false) {
		printf("stamon: compile: %d fatal error(s):\n", stamon.ErrorMsg.size());
		for (int i = 0, len = stamon.ErrorMsg.size(); i < len; i++) {
			printf("%s\n", stamon.ErrorMsg[i].getstr());
		}
		return -1;
	}

	return 0;
}

int StamonRunCommand(ArrayList<String> args) {
	checkEnvironmentVariableWarning();

	String src;

	// 先设置为配置文件的默认值
	int warning_level = stamon::vm::config::WarningLevel;
	bool isGC = stamon::vm::config::isGC;
	int MemLimit = stamon::vm::config::MemLimit;
	int PoolCacheSize = stamon::vm::config::PoolCacheSize;

	if (args.size() < 2) {
		printf("stamon: run: too few arguments\n"
			   "please enter \'stamon help\' "
			   "to get more information.\n");
		return -1;
	} else {
		src = args[1];

		for (int i = 2, len = args.size(); i < len; i++) {
			if (args[i].equals(String((char *) "--GC=true"))) {
				isGC = true;
			} else if (args[i].equals(String((char *) "--GC=false"))) {
				isGC = false;
			} else if (args[i].length() > 11
					   && args[i].substring(0, 11).equals(
							   String((char *) "--MemLimit="))) {
				MemLimit = args[i].substring(11, args[i].length()).toInt();
			} else if (args[i].length() > 15
					   && args[i].substring(0, 15).equals(
							   String((char *) "--MemPoolCache="))) {
				PoolCacheSize = args[i].substring(15, args[i].length()).toInt();
			} else if (args[i].equals(String((char *) "--IgnoreWarning"))) {
				warning_level = StamonWarningSafeLevel_IgnoreWarning;

			} else if (args[i].equals(String((char *) "--JustWarn"))) {
				warning_level = StamonWarningSafeLevel_JustWarn;

			} else if (args[i].equals(String((char *) "--FatalWarning"))) {
				warning_level = StamonWarningSafeLevel_FatalWarning;

			} else if (args[i].length() >= 10
					   && args[i].substring(0, 9).equals((char *) "--locale=")) {
				// 设置语言环境
				setlocale(
						LC_ALL, args[i].substring(9, args[i].length()).getstr());

			} else {
				printf("stamon: run: bad command\n"
					   "please enter \'stamon help\' "
					   "to get more information.\n");
				return -1;
			}
		}
	}

	Stamon stamon;

	stamon.init();

	stamon.run(src, isGC, MemLimit, PoolCacheSize);

	if (stamon.WarningMsg.empty() == false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning) {
		if (warning_level == StamonWarningSafeLevel_JustWarn) {
			printf(
				"stamon: run: %d warning(s):\n",
				stamon.WarningMsg.size()
			);
		} else if (warning_level == StamonWarningSafeLevel_FatalWarning) {
			printf(
				"stamon: compile: %d fatal warning(s):\n",
				stamon.WarningMsg.size()
			);
		}
		for (int i = 0, len = stamon.WarningMsg.size(); i < len; i++) {
			printf("%s\n", stamon.WarningMsg[i].getstr());
		}
		if (warning_level == StamonWarningSafeLevel_FatalWarning) {
			return -1;
		}
	}

	if (stamon.ErrorMsg.empty() == false) {
		printf("stamon: run: %d fatal error(s):\n", stamon.ErrorMsg.size());
		for (int i = 0, len = stamon.ErrorMsg.size(); i < len; i++) {
			printf("%s\n", stamon.ErrorMsg[i].getstr());
		}
		return -1;
	}

	return 0;
}

int StamonStripCommand(ArrayList<String> args) {
	checkEnvironmentVariableWarning();

	String src;

	// strip功能的默认警告重要程度与编译器一致
	int warning_level = stamon::c::config::WarningLevel;

	if (args.size() < 2) {
		printf("stamon: strip: too few arguments\n"
			   "please enter \'stamon help\' "
			   "to get more information.\n");
		return -1;
	} else {
		src = args[1];
		for (int i = 2; i < args.size(); i++) {
			if (args[i].equals(String((char *) "--IgnoreWarning"))) {
				warning_level = StamonWarningSafeLevel_IgnoreWarning;

			} else if (args[i].equals(String((char *) "--JustWarn"))) {
				warning_level = StamonWarningSafeLevel_JustWarn;

			} else if (args[i].equals(String((char *) "--FatalWarning"))) {
				warning_level = StamonWarningSafeLevel_FatalWarning;
			}
		}
	}

	Stamon stamon;

	stamon.init();

	stamon.strip(src);

	if (stamon.WarningMsg.empty() == false
			&& warning_level != StamonWarningSafeLevel_IgnoreWarning) {
		if (warning_level == StamonWarningSafeLevel_JustWarn) {
			printf(
				"stamon: strip: %d warning(s):\n",
				stamon.WarningMsg.size()
			);
		} else if (warning_level == StamonWarningSafeLevel_FatalWarning) {
			printf(
				"stamon: strip: %d fatal warning(s):\n",
				stamon.WarningMsg.size()
			);
		}
		for (int i = 0, len = stamon.WarningMsg.size(); i < len; i++) {
			printf("%s\n", stamon.WarningMsg[i].getstr());
		}
		if (warning_level == StamonWarningSafeLevel_FatalWarning) {
			return -1;
		}
	}

	if (stamon.ErrorMsg.empty() == false) {
		printf("stamon: strip: %d fatal error(s):\n", stamon.ErrorMsg.size());
		for (int i = 0, len = stamon.ErrorMsg.size(); i < len; i++) {
			printf("%s\n", stamon.ErrorMsg[i].getstr());
		}
		return -1;
	}

	return 0;
}