#!/bin/bash
# 使用脚本来辅助编译和运行示例程序
echo "Test to build a example"
echo "--------------------------"

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
# 必要的环境变量
export STAMON=${SHELL_FOLDER}/../bin
STAMON_EXE=${STAMON}/stamon

cd ${SHELL_FOLDER}

$STAMON_EXE -b main.st main.stvc -I./
$STAMON_EXE -r main.stvc