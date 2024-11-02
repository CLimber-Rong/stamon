# STVC-TAC二进制编码规范（第一版）

### 前言

在阅读本规范前，我们强烈建议先阅读``STVC-TAC中间代码规范.md``

由于``STVC-TAC中间代码规范.md``并没有对具体编码细节进行阐述，因此编写此文档进行补充。

此文档仅作规范，并非唯一解决方案，开发者也可以制定自己的编码格式（但我们强烈建议制定的规范符合``STVC-TAC中间代码规范``）

### 常量表

一个常量表由常量表长度（占4字节）和若干个常量组成，其中常量又由常量类型（占1字节）和常量值组成：

其中每种类型的编码格式如下：

* 整数类型：占4字节，即数值
* 单精度浮点类型：占4字节，即数值
* 双精度浮点类型：占8字节，即数值
* 字符串类型：占4+l字节，其中前4字节记录字符串长度（即l，长度按字节计），后l字节为字符串值
* 标识符类型：占4+l字节，其中前4字节记录标识符长度（即l，长度按字节计），后l字节为标识符名


### 语句

语句的操作码占一字节，操作数不定长。

以下为所有的基本语句，其中第一条语句对应的操作码为``0x01``，第二条对应``0x02``，以此类推。

注意：常量表下标默认为4字节。

* ``x = ASSIGN y``：操作数为x和y的常量表下标
* ``x = UNARY op y``：操作数为x和y的常量表下标
* ``x = BINARY y op z``：操作数为x和y和z的常量表下标
* ``goto addr``：操作数为4字节的数值，代表跳转地址
* ``if condition => addr``：操作数为condition的常量表下标和4字节的数值（跳转地址）
* ``call result function: arg1 arg2 ...``：操作数为result的常量表下标，和4字节的数值（参数个数）以及所有参数的常量表下标
* ``return value``：操作数为value的常量表下标
* ``try addr``：操作数为4字节的数值，代表跳转地址
* ``new object source arg1 arg2 ...``：操作数为object的常量表下标，和4字节的数值（构造参数个数）以及所有参数的常量表下标
* ``member dst src mber``：操作数为dst和src和mber的常量表下标
* ``sfn port arg``：操作数为port和arg的常量表下标
* ``list identifier element1 element2...``：操作数为identifier的常量表下标，和4字节的数值（元素个数）以及所有元素的常量表下标
* ``pushscope``：无操作数
* ``popscope``：无操作数
* ``free identifier``：操作数为identifier的常量表下标
* ``getexception identifier``：操作数为identifier的常量表下标
* ``resetexceptionflag``：无操作数
  