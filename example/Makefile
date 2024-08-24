# 为什么要有这个 makefile？
# 这是一个编译的示例
# 我们可以通过这样的写法来编译st源文件，并生成stvc文件
# 然后运行stamon来运行stvc文件

MAKEFILE_PATH = $(shell pwd)
STAMON_PATH = ${MAKEFILE_PATH}/../bin

STAMON_EXE = ${STAMON_PATH}/stamon
# 必须导出的环境变量
export STAMON=${STAMON_PATH}
# 导入的文件
imports = ${MAKEFILE_PATH}
# 需要运行的文件：注不带后缀
R = ""
# 生成
%.stvc: %.st 
	${STAMON_EXE} -b $< $@ -I${imports}
	@echo
# 运行
run: ${R}.stvc
	@${STAMON_EXE} -r ${R}.stvc 
# 获取帮助
help:
	${STAMON_EXE} help