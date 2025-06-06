# STVC-TAC二进制编码规范（第二版）

### 前言

在阅读本规范前，我们**强烈建议**先阅读``STVC-TAC中间代码规范.md``。

由于``STVC-TAC中间代码规范.md``并没有对具体编码细节进行阐述，因此编写此文档进行补充。

此文档仅作规范，并非唯一解决方案，开发者也可以制定自己的编码格式（但我们强烈建议制定的规范符合``STVC-TAC中间代码规范``）。

### 常量表

一个常量表由常量表长度（占4字节）和若干个常量组成，其中常量又由常量类型（占1字节）和常量值组成：

其中每种类型的编码格式如下：

* 整数类型：占4字节，即数值
* 单精度浮点类型：占4字节，即数值
* 双精度浮点类型：占8字节，即数值
* 字符串类型：占4+len字节，其中前4字节记录字符串长度（即l，长度按字节计），后len字节为字符串值
* 标识符类型：占4+len字节，其中前4字节记录标识符长度（即len，长度按字节计），后len字节为标识符名

一个常量表下标默认为4字节。

### 语句

其中第一条语句对应的操作码为``0x01``，第二条对应``0x02``，以此类推。

这里需要特别说明一下，二进制布局的表示形式，操作数的二进制布局会用如下格式表示

``OPND1(size) OPND2(size) ...``

即每一个操作数的名字后面跟着这个操作数的长度。

在了解了二进制布局的格式之后，我们来看看每条中间代码语句对应的二进制布局

|中间代码|对应的操作数二进制布局|补充解释|
|:-|:-|:-|
|``x = ASSIGN y``|``x(4) y(4)``|x和y都是常量表下标|
|``x = UNARY op y``|``x(4) op(4) y(4)``|x和y是常量表下标，op是一个整数|
|``x = BINARY y op z``|``x(4) op(4) y(4) z(4)``|x和y和z是常量表下标，op是一个整数|
|``x[index] = y``|``x(4) index(4) y(4)``|x和y和index都是常量表下标|
|``x = y[index]``|``x(4) index(4) y(4)``|x和y和index都是常量表下标|
|``x.member = y``|``x(4) member(4) y(4)``|x和y和member都是常量表下标|
|``x = y.member``|``x(4) member(4) y(4)``|x和y和member都是常量表下标|
|``goto addr``|``addr(4)``|addr代表跳转地址，是一个整数|
|``if condition => addr``|``condition(4) addr(4)``|condition是一个常量表下标，addr是一个整数|
|``call result function arg1 arg2 ...``|``result(4) function(4) arglen(4) arg1(4) arg2(4) ...``|result、function、arg1、arg2等是常量表下标，arglen是一个整数，代表参数的个数|
|``return value``|``value(4)``|value是一个常量表下标|
|``new object source arg1 arg2 ...``|``object(4) source(4) arglen(4) arg1(4) arg2(4) ...``|object、source、arg1、arg2等都是常量表下标，arglen是一个整数，代表参数的个数|
|``list identifier element1 element2...``|``identifier(4) arglen(4) element1(4) element2(4) ...``|identifier、element1、element2等都是常量表下标，arglen是一个整数，代表参数的个数|
|``array identifier length``|``identifier(4) length(4)``|identifier和length都是常量表下标|
|``free identifier``|``identifier(4)``|identifier是一个常量表下标|
|``pushscope``|``nothing(0)``|没有任何操作数|
|``popscope``|``nothing(0)``|没有任何操作数|
|``def identifier``|``identifier(4)``|identifier是一个常量表下标|
|``pushcatch addr``|``addr(4)``|addr是一个整数|
|``popcatch``|``nothing(0)``|没有任何操作数|
|``getexception identifier``|``identifier(4)``|identifier是一个常量表下标|
|``sfn port arg``|``port(4) arg(4)``|port和arg都是常量表下标|
|``function identifier arg1 arg2...``|``identifier(4) arglen(4) arg1(4) arg2(4) ...``|identifier、arg1、arg2等都是常量表下标，arglen是一个整数，代表参数的个数|
|``class identifier member1 member2 * member3...``|``identifier(4) member1_methodflag(1) member1(4) member2_methodflag(1) member2(4) member3_methodflag(1) member3(4) ...``|identifier、member1、member2、member3等都是常量表下标，member1_methodflag、member2_methodflag、member3_methodflag等都是整数，表示其对应的类成员是否是类方法|
|``end``|``nothing(0)``|没有任何操作数|