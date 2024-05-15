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
	-I src/sfn \
	-I src \
	-lm

zip: test.exe
	$(STRIP) -s test.exe
	$(UPX) -9 test.exe

run: test.exe
	./test.exe

release:
	$(COMPILER) src/Main.cpp \
	-o bin/stamon.exe \
	-O2 \
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

	$(STRIP) -s bin/stamon.exe
	$(UPX) -9 bin/stamon.exe

debug: bin/stamon.exe
	bin/stamon.exe build code.st code.stvc