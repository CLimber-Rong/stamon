# AstRunner开发文档

> 注意：此文档旨在说明``AstRunner``（以下简称“虚拟机”）的工作原理

虚拟机的运行原理为：将二进制文件读取为AstIr，交给AstIrConverter类解析为Running-Ast，最后交给``vm/AstRunner.cpp``递归运行。

我们规定：Stamon编译后的二进制文件为**STVC文件**，文件后缀为``.stvc``

我们来逐步讲解。

首先是二进制文件读取为AstIr，这部分的实现位于``src/action/AstIrReader.cpp``，AstIrReader类的主要接口有：

* ``AstIrReader(log::Exception *e, String filename)``：构造函数，filename为文件名
* ``bool readHeader()``：读取字节码头，读取失败则抛出异常并返回false，否则返回true
* ``ArrayList<AstIr> ReadIR()``：读取AstIr，返回由AstIr组成的ArrayList

想要完整的读取一个STVC文件，应该要先创建一个AstIrReader对象，然后先调用``readHeader``读取文件头信息，接着调用``readIR``来读取AstIr。调用这两个函数之后要分别检查是否有异常抛出。

接着是让AstIrConverter类解析为Running-Ast，这一部分在**写了Ast与AstIr之间的互转工具**部分里已经详细提及过了，故不再赘述。

最后是交给``vm/AstRunner.cpp``递归运行，``AstRunner``类采用了和语法分析器类似的结构，下面我们来看看重点的数据接口及接口：

AstRunner在递归执行Ast时的返回值为``RetStatus``类。RetStatus，全称*Return-Status（返回状态）*，用于指示当前代码运行状况，我们来看看RetStatus的定义：

```C++
class RetStatus {	//返回的状态（Return Status）
        //这个类用于运行时
    public:
        int status;	//状态码
        VariablePtr retval;	//返回值（Return-Value），无返回值时为NULL
        RetStatus() : retval(NullVariablePtr()) {}
        RetStatus(const RetStatus& right) : retval(right.retval) {
            status = right.status;
        }
        RetStatus(int status_code, VariablePtr retvalue) 
        : retval(retvalue) {
            status = status_code;
        }
};
```

其中的``int status``一行用于存储状态码，状态码有以下几类：

```C++
enum RET_STATUS_CODE {	//返回的状态码集合
    RetStatusErr = -1,	//错误退出（Error）
    RetStatusNor,		//正常退出（Normal）
    RetStatusCon,		//继续循环（Continue）
    RetStatusBrk,		//退出循环（Break）
    RetStatusRet		//函数返回（Return）
};
```

AstRunner的主要接口有：

```C++
RetStatus execute(
    AstNode* main_node, bool isGC, int vm_mem_limit,
    ArrayList<DataType*> tableConst, ArrayList<String> args,
    log::Exception* e
);
```

虚拟机在执行过程中会向ObjectManager申请对象，来实现GC机制。详见``对象管理器文档.md``