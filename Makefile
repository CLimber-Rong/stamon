# 可以更改$(COMPILER)以及$(REMOVE)来自定义编译环境
COMPILER = g++
STRIP = strip
UPX = upx
REMOVE = del

STD = c++17
LINK = 	-I include/stdc_implemented \
		-I src/ast \
		-I src/data_type \
		-I src/vm \
		-I src/ir \
		-I src/compiler \
		-I src/sfn \
		-I src/tac \
		-I src/action \
		-I src/exception \
		-I src/config \
		-I src \
		-lm

# 以下指令用于编译发行版
# -static选项让编译的程序更通用，但是程序体积更大，删去可以缩小程序体积

release:
# 自定义生成可执行文件名（默认为stamon.exe）

	xcopy src\bin-include bin\include /s /e /y /i
	$(COMPILER) src/Main.cpp \
	-o bin/stamon.exe \
	-O2 \
	-std=$(STD) \
	-static \
	$(LINK)

	$(STRIP) -s bin/stamon.exe

release_win:
#	编译Windows版本

	xcopy src\bin-include bin\include /s /e /y /i
	$(COMPILER) src/Main.cpp \
	-o bin/stamon.exe \
	-O2 \
	-std=$(STD) \
	-static \
	$(LINK)

	$(STRIP) -s bin/stamon.exe

release_linux:
#	编译Linux版本
	mkdir -p "bin/include"
	cp -r -T src/bin-include bin/include
	$(COMPILER) src/Main.cpp \
	-o bin/stamon \
	-O2 \
	-std=$(STD) \
	-static \
	$(LINK)

	$(STRIP) -s bin/stamon

release_macos:
#	编译MacOS版本
	mkdir -p "bin/include"
	cp -r "src/bin-include/" "bin/include"
	$(COMPILER) src/Main.cpp \
	-o bin/stamon \
	-O2 \
	-std=$(STD) \
	$(LINK)

	$(STRIP) bin/stamon

zip_release_win:
	$(UPX) --best --lzma bin/stamon.exe

zip_release_linux:
	$(UPX) --best --lzma bin/stamon.exe

zip_release_macos:
	$(UPX) --best --lzma bin/stamon.exe