COMPILER = g++
STRIP = strip
UPX = upx
REMOVE = del


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
	-lm

zip: test.exe
	$(STRIP) -s test.exe
	$(UPX) -9 test.exe

run: test.exe
	./test.exe