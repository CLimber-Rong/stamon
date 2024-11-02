# STVC-TAC中间代码规范

### 前言

##### 什么是STVC-TAC？

``STVC-TAC``（以下简称``STVC``）是一种以三地址码为基础的平面字节码规范。

> ``STVC-TAC``区别于``AST-IR``，前者的字节码是线性且平面的，而后者则是递归且树状的。

##### 为什么要设计STVC-TAC？

比起形似AST的``AST-IR``，``STVC``更容易优化，且执行速度相对更快。

### STVC文件结构

STVC文件由以下部分组成：

1. 魔数（0xABDB）
2. 常量表
3. 代码声明
4. 入口代码

### 常量表格式

一个常量表由常量表长度（占4字节）和若干个常量组成，其中常量又由常量类型（占1字节）和常量值组成：

其中常量值有以下类型：

* 整数类型
* 单精度浮点类型
* 双精度浮点类型
* 字符串类型
* 标识符类型

将所有涉及到的数据和标识符存入常量表，这样在字节码代码中，若涉及到某数值或标识符，只需指定一个下标，虚拟机就能通过下标在常量表中查找出对应的数值或标识符。这么做极大的减少了冗余数据的存储，减小了程序体积。

**注意：按照规定，常量表的第一条常量必须是一个名为``__init__``的标识符**

### 基本概念和基本语句

##### 标识符

标识符有三个种类：用户标识符，临时标识符和内部标识符。

用户标识符一般为用户自定义的标识符，其格式与C标识符格式相同。

临时标识符则是表达式计算过程当中会用到的标识符，此类标识符的格式为``.XXX``，其中“XXX”为数字。

内部标识符则是用于匿名类、匿名函数的声明，此类标识符的格式为``#XXX``，其中“XXX”为数字。

因此编译器在编译时应先指明标识符的类型，再指明标识符的编号。

##### 运算语句

1. ``x = ASSIGN y``

将y赋值给x。

2. ``x = UNARY op y``

将y进行单目运算（运算符为op）之后的值传入x。当op为``arr``时，代表创建一个长度为y的数列。

3. ``x = BINARY y op z``

将y和z进行双目运算（运算符为op）之后的值传入x。

上述的ASSIGN、UNARY和BINARY均为助记符，虚拟机可以通过助记符直接判断运算类型。助记符应当在二进制文件中表现出来。

##### 流程控制语句

1. ``goto addr``

无条件跳转至相对addr行指令所在处。若addr<0，向上跳转，否则向下跳转。

2. ``if condition => addr``
   
如果condition不为``null``或``0``则跳转至相对addr行指令所在处。若addr<0，向上跳转，否则向下跳转。

3. ``call result function: arg1 arg2 ...``

调用function值。参数为arg1、arg2等标识符或值。返回值存入result当中。

4. ``return value``

返回value值。

5. ``try addr``

监测该以下addr（addr必须是正数）行的代码是否有抛出异常，如果出现异常，就直接跳转到``try``指令以下addr行（即跳过try代码块），因此try代码块在包含本应包含的代码的同时，也要在结尾加上``goto catch代码块大小``，以此跳过catch块；在try代码块后需要紧跟着catch代码块。


值得注意的是：
* 入口代码不需要在结尾返回
* 跳转的指令偏差应该是能接受的（如一个函数有五条指令，显然不能往下跳转六条指令）

##### 特别的...

1. ``new object source arg1 arg2 ...``

将source标识符作为类，新建对象，构造参数为arg1、arg2等，新建后的对象值存入object标识符

2. ``member dst src mber``

将src中的mber成员取出，存入dst。

3. ``sfn port arg``

设置SFN，port为端口号标识符。arg参数标识符。

SFN的介绍见``编译器开发文档.md``

4. ``list identifier element1 element2...``

将element1、element2...作为元素，组合成数列，并存入identifier标识符中

5. ``pushscope``

压入一个作用域，用于跳转指令

6. ``popscope``

弹出一个作用域，用于跳转指令

7. ``free identifier``

如果identifier所存储的是字面量值（如整型），则释放。该指令和C语言的register关键字类似，**是否释放取决于虚拟机状态。**该指令通常用于释放临时标识符。

8. ``throw identifier``

将identifier标识符所存储的值作为异常内容，进行抛出。

9. ``getexception identifier``

将当前异常内容存入identifier。

10. ``resetexceptionflag``

重置异常标记（即标记为无异常状态）。

### 代码声明

代码声明部分包含了函数定义和类定义。虚拟机在执行入口代码时会先初始化这些定义的对象。

##### 声明函数

```
function identifier: arg1 arg2 arg3 ...
...some codes...
end
```

其中identifier为函数名。arg1、arg2、arg3等为函数的参数名。“...some codes...”为函数体代码

函数内部不能嵌套声明函数或类（因此需要在函数外部，用内部标识符定义函数或类，在运行时赋值）。

##### 声明类

```
class identifier: member1 member2 member3 ...
...some codes...
method member1 member2 ... 
end
```

其中identifier为类名。member1、member2、member3等为类成员名。“...some codes...”为类初始化赋值代码（并不是构造函数）。

method语句包含了若干个标识符，表示此标识符为方法。

在初始化类对象时，会先执行初始化赋值代码，根据method语句为方法指定容器，再调用构造函数。

类内部不能嵌套声明函数或类（因此需要在在外部用内部标识符定义函数或类，在初始化时赋值）。

### 入口代码

全局编写的代码就是入口代码，编译器需要整理入口代码并集中放置在可执行文件的末尾。

### 附表

所有基本语句有以下几种：

* ``x = ASSIGN y``
* ``x = UNARY op y``
* ``x = BINARY y op z``
* ``goto addr``
* ``if condition => addr``
* ``call result function: arg1 arg2 ...``
* ``return value``
* ``try addr``
* ``new object source arg1 arg2 ...``
* ``member dst src mber``
* ``sfn port arg``
* ``list identifier element1 element2...``
* ``pushscope``
* ``popscope``
* ``free identifier``
* ``getexception identifier``
* ``resetexceptionflag``


双目运算符有以下种类：

* add：加
* sub：减
* mul：乘
* div：除
* mod：取余
* lsh：左移
* rsh：右移
* less：小于
* lequ：小等于
* big：大于
* bequ：大等于
* equ：判等
* iequ：判不等
* band：按位与
* bxor：按位异或
* bor：按位或
* land：逻辑与
* lor：逻辑或
* index：取下标

而单目运算符有以下种类：

* pos：正
* neg：负
* cpl：按位取反
* not：逻辑非
* arr：组成为数列