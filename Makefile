# 可以更改$(COMPILER)以及$(REMOVE)来自定义编译环境
COMPILER = g++
STRIP = strip
UPX = upx
REMOVE = del

# 以下指令用于兼容先前调试时的编译指令

clean:
	- $(REMOVE) test.exe

build: clean
	$(COMPILER) test/test.cpp \
	-o test.exe \
	-O0 \
	-std=c++11 \
	-I include/stdc_implemented \
	-I src/ast \
	-I src/data_type \
	-I src/vm \
	-I src/ir \
	-I src/compiler \
	-I src/sfn \
	-I src \
	-lm

zip: test.exe
	$(STRIP) -s test.exe
	$(UPX) -9 test.exe

run: test.exe
	./test.exe

debug: bin/stamon.exe
	bin/stamon.exe build code.st code.stvc



# 以下指令用于编译发行版


release:
# 自定义生成可执行文件名（默认为stamon.exe）

	xcopy src\bin-include bin\include /s /e /y /i
	$(COMPILER) src/Main.cpp \
	-o bin/stamon.exe \
	-O2 \
	-std=c++17 \
	-I include/web_implemented \
	-I src/ast \
	-I src/data_type \
	-I src/vm \
	-I src/ir \
	-I src/compiler \
	-I src/sfn \
	-I src/tac \
	-I src \
	--js-library include/.js \
	-lm

	$(STRIP) -s bin/stamon.exe
	$(UPX) -9 bin/stamon.exe

release_win:

	xcopy src\bin-include bin\include /s /e /y /i
	$(COMPILER) src/Main.cpp \
	-o bin/stamon.exe \
	-O2 \
	-std=c++17 \
	-I include/stdc_implemented \
	-I src/ast \
	-I src/data_type \
	-I src/vm \
	-I src/ir \
	-I src/compiler \
	-I src/sfn \
	-I src/tac \
	-I src \
	-lm

	$(STRIP) -s bin/stamon.exe
	$(UPX) -9 bin/stamon.exe

release_linux:
#	编译Linux版本
	cp -r -T src/bin-include bin/include
	$(COMPILER) src/Main.cpp \
	-o bin/stamon \
	-O2 \
	-std=c++17 \
	-I include/stdc_implemented \
	-I src/ast \
	-I src/data_type \
	-I src/vm \
	-I src/ir \
	-I src/compiler \
	-I src/sfn \
	-I src/tac \
	-I src \
	-lm

	$(STRIP) -s bin/stamon
	$(UPX) -9 bin/stamon