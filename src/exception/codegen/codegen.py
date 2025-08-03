"""
Name: codegen.py
License: Apache 2.0
Author: CLimber-Rong
Date: 07/07/25 17:52
Description: 报错信息及文档的自动生成器
"""

import time
import json

doc_path = "../../../doc/Stamon异常信息修复指南.md"
# 生成的文档路径

setting_list = [
    ("astrunner", "CLimber-Rong"),
    ("compiler", "CLimber-Rong"),
    ("astfilereader", "CLimber-Rong"),
    ("astir", "CLimber-Rong"),
    ("astirreader", "CLimber-Rong"),
    ("objectmanager", "CLimber-Rong"),
    ("tokenfilereader", "CLimber-Rong"),
    ("sfn", "CLimber-Rong")
]
# 生成所需的配置名称及其对应作者


def gen_exception_func(f, sender: str) -> str:
    # 生成函数头
    code = "inline STMInfo " + f["type"] + "(String position"

    for arg in f["arg"]:
        code += ", String " + arg

    code += ") {\n\t"

    # 生成报错消息
    msg = ""
    first_flag = True  # 第一个字符串前面不能有加号
    for m in f["msg"]:
        if first_flag is False:
            msg += "+"
        else:
            first_flag = False
        if m[0] == "$":
            msg += m[1:]  # 参数
        else:
            msg += 'String("' + m + '")'

    # 整理并生成整个函数
    code += (
        "return STMInfo("
        + 'String("'
        + sender
        + '"), '
        + 'String("'
        + f["type"]
        + '"), '
        + msg
        + ", position);\n}\n\n"
    )

    return code


def gen_codefile(setting_name: str, author: str) -> None:
    """打开配置文件并生成对应的代码"""
    # 打开json文件
    fin = open(setting_name + ".json", "r", encoding="utf8")
    setting = json.loads(fin.read())
    fin.close()

    # 生成文件头
    timestr = time.strftime("%d/%m/%Y %H:%M", time.localtime())
    fileheader = (
        "/*\n\tName: "
        + setting["name"]
        + "\n\tLicense: Apache 2.0\n\tAuthor: "
        + author
        + "\n\tDate: "
        + timestr
        + "\n\tDescription: 报错信息函数，由codegen.py自动生成"
        + "\n*/\n\n"
        + "#pragma once\n"
        + '#include "String.hpp"\n'
        + '#include "Exception.hpp"\n\n'
    )

    # 生成代码
    code = "namespace stamon::exception::" + setting_name + " {\n"
    for f in setting["error"]:
        code += gen_exception_func(f, setting_name)
    for f in setting["warning"]:
        code += gen_exception_func(f, setting_name)

    code += "}"

    # 写入到对应文件
    fout = open("../" + setting["name"], "w", encoding="utf8")
    fout.write(fileheader + code)
    fout.close()


def gen_func_doc(f) -> str:
    return "##### " + f["type"] + "\n\n" + f["doc"] + "\n\n"


def gen_doc(setting_name: str) -> str:
    """打开配置文件并生成对应的文档"""
    # 打开json文件
    fin = open(setting_name + ".json", "r", encoding="utf8")
    setting = json.loads(fin.read())
    fin.close()

    doc = "### " + setting_name + "\n\n"
    for i in setting["error"]:
        doc += gen_func_doc(i)
    for i in setting["warning"]:
        doc += gen_func_doc(i)

    return doc


if __name__ == "__main__":
    # 遍历配置列表
    doc = "# Stamon错误及警告信息修复指南\n\n> 本文件为``src/exception/codegen/codegen.py``根据配置文件自动生成，用户可以用搜索文本的方式快速定位自己的问题\n\n"
    for i in setting_list:
        gen_codefile(i[0], i[1])
        doc += gen_doc(i[0])

    fout = open(doc_path, "w", encoding="utf8")
    fout.write(doc)
    fout.close()
