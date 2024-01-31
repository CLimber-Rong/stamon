'''
Name: generator.py
Copyright: Apache 2.0
Author: CLimber-Rong
Date: 07/08/23 13:15
Description: 由于节点声明的代码又多又相近，所以我尝试用宏来快速编写ast
             但是经过尝试之后，我发现宏解决不了我的问题
             于是，code_generator目录诞生了
             generator.py根据setting.json生成ast节点代码
             generator.py会把生成的代码全部输出到同目录的code.cpp里
             我会人工的把code.cpp里的代码复制粘贴到ast目录下的各个源码中
             这样做会快很多
'''

# 如果你不是来了解项目制作过程/试图改装虚拟机的，可以忽略code_generator目录

import json

# 当然，作为一个简单的生成器，我不会使用面向对象

def MakeClassCode(AstName: str, PrivateMembers: str, PublicMembers: str, Structures: list) -> str:
    '''
    根据参数生成源码，返回一个字符串

    AstName是节点的名字，不需要“Ast”前缀，

    PrivateMembers是私有成员的代码，

    PublicMembers是公共成员的代码，

    Structures是构造函数的代码。

    这里着重介绍Structures，由于一个类可能会有多种构造函数，所以Structures是一个存着所有构造函数的列表。

    Structures里的每个元素都是一个字典————每个字典都代表着一个构造函数。

    字典的['args']是一个字符串————构造函数的参数代码，前后不需要加括号。

    字典的['code']是一个字符串————构造函数的实现代码，前后不需要加花括号

    '''
    code = ''
    code += "class " + "Ast" + AstName + " : public AstNode {\n"
    code += PrivateMembers + "\n"
    code += "public:\n"
    code += PublicMembers + "\n"

    for struct in Structures:
        code += "Ast" + AstName
        code += "(" + struct['args'] + ")"
        code += " : AstNode()\n"
        code += "{" + struct['code'] + "}" + "\n"
    
    code += "virtual int getType() {" + "\n"
    code += "return " + "Ast" + AstName + "Type;" + "\n" + "}"
    
    code += "};"

    return code

def MakeCode() -> str:
    '''
    根据同目录的setting.json生成代码
    '''
    setting = json.load(open('src/ast/code_generator/setting.json','r'))
    code = ''
    ast_class = setting['ast_class']
    for cls in ast_class:
        name = cls['name']
        
        private_members = ''    # private_members是默认没有的
        if 'private_members' in cls:
            private_members = cls['private_members']
        
        public_members = ''     # public_members是默认没有的
        if 'public_members' in cls:
            public_members = cls['public_members']
        
        structures = cls['structure']
        
        code += MakeClassCode(name, private_members, public_members, structures)
    return code

if __name__=='__main__':
    # 如果你直接打开生成的code.cpp，可能会出来几十个报错，这是正常的
    # 原因是我并没有在code.cpp里引用Ast.hpp，所以很多标识符vscode不认识
    # 当你把code.cpp里的内容粘贴到其他ast源码当中时，报错就消失了
    # 我已经把源码粘贴到其他ast源码当中
    code = MakeCode()
    file = open('src/ast/code_generator/code.cpp','w')
    file.write(code)
    file.close()