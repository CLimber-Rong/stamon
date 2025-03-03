# STVC-TAC中间代码规范（第二版）

### 前言

##### 什么是STVC-TAC？

``STVC-TAC``（又称``TAC-IR``，以下简称``STVC``）是一种以三地址码为基础的平面字节码规范。

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

**用户标识符**一般为用户自定义的标识符，其格式与C标识符格式相同。

**临时标识符**则是表达式计算过程当中会用到的标识符，此类标识符的格式为``.XXX``，其中“XXX”为数字。

**内部标识符**则是用于匿名类、匿名函数的声明，此类标识符的格式为``#XXX``，其中“XXX”为数字。

因此编译器在编译时应先指明标识符的类型，再指明标识符的编号。

##### SFN

SFN，全称*Stamon For Native*，是Stamon调用外部库的一个机制。调用SFN时，需要指定一个端口并提供参数，不同的端口对应不同的外部功能。

##### 容器

“容器”的概念与“成员”的概念恰好相反。

如果一个对象 *O* 拥有一个成员 *m*，那么规定 *O* 是 *m* 的容器。

初始化一个类对象时，通常需要初始化类方法。
而初始化类方法时，通常需要为其指定容器。这样用户在调用类方法时，Stamon会自动将容器传入“self”参数，从而实现类方法调用其他类成员。

类方法和全局函数在数据类型上是相同的。唯一不同之处在于：类方法拥有容器，而全局函数不具有容器。

##### 运算语句

1. ``x = ASSIGN y``

将y赋值给x。

2. ``x = UNARY op y``

将y进行单目运算（运算符为op）之后的值传入x。当op为``arr``时，代表创建一个长度为y的数列。

3. ``x = BINARY y op z``

将y和z进行双目运算（运算符为op）之后的值传入x。

上述的ASSIGN、UNARY和BINARY均为助记符，虚拟机可以通过助记符直接判断运算类型。助记符应当在二进制文件中表现出来。

4. ``x[index] = y``

将x的第index个元素赋值为y。

5. ``x = y[index]``

将x赋值为y的第index个元素。

6. ``x.member = y``

将x的member成员赋值为y。

7. ``x = y.member``

将x赋值为y的member成员。

##### 流程控制语句

1. ``goto addr``

无条件跳转至相对addr行指令所在处。若addr<0，向上跳转，否则向下跳转。

2. ``if condition addr``
   
如果condition不为``null``或``0``则跳转至相对addr行指令所在处。若addr<0，向上跳转，否则向下跳转。

3. ``call result function arg1 arg2 ...``

调用function值。参数为arg1、arg2等标识符或值。返回值存入result当中。

4. ``return value``

返回value值。


值得注意的是：
* 入口代码不需要在结尾返回
* 跳转的指令偏差应该是能接受的（如一个函数有五条指令，显然不能往下跳转六条指令）

##### 内存操作语句

1. ``new object source arg1 arg2 ...``

将source标识符作为类，新建对象，构造参数为arg1、arg2等，新建后的对象值存入object标识符。

2. ``list identifier element1 element2...``

将element1、element2...作为元素，组合成数列，并存入identifier标识符中。

3. ``array identifier length``

将identifier赋值为一个长度为length的空数列。

4. ``free identifier``

如果identifier所存储的是字面量值（如整型），则释放。该指令和C语言的register关键字类似，**是否释放取决于虚拟机状态。**该指令通常用于释放临时标识符。

##### 作用域操作语句

1. ``pushscope``

压入一个作用域，用于跳转指令。

2. ``popscope``

弹出一个作用域，用于跳转指令。

3. ``def identifier``

在当前作用域定义一个名为identifier的变量。

##### 异常处理语句

1. ``pushcatch addr``

进入一个新的try-catch块时，需要调用该指令，addr为catch块的地址。

2. ``popcatch``

退出一个try-catch块时，需要调用该指令。

3. ``getexception identifier``

将当前异常内容存入identifier。

##### 外部库调用语句

1. ``sfn port arg``

调用SFN的port端口，参数为arg。

### 代码声明

代码声明部分包含了函数定义和类定义。虚拟机在执行入口代码时会先初始化这些定义的对象。

##### 声明函数

```
function identifier arg1 arg2 ...
...some codes...
end
```

其中identifier为函数名。arg1、arg2等为函数的参数名。“...some codes...”为函数体代码。

函数内部不能嵌套声明函数或类（因此需要在函数外部，用内部标识符定义函数或类，在运行时赋值）。

##### 声明类

```
class identifier member1 member2 * member3...
...some codes...
end
```

其中identifier为类名。member1、member2、member3等为类成员名。“...some codes...”为类初始化赋值代码（并不是构造函数）。

如果类成员名的左侧带有“*”，则代表此成员是类方法。

在初始化类对象时，会先执行初始化赋值代码，接着为类方法指定容器，再调用构造函数。

类内部不能嵌套声明函数或类（因此需要在在外部用内部标识符定义函数或类，在初始化时赋值）。

### 入口代码

全局编写的代码就是入口代码，编译器需要整理入口代码并集中放置在可执行文件的末尾。

### 附表

所有语句有以下几种：

* ``x = ASSIGN y``
* ``x = UNARY op y``
* ``x = BINARY y op z``
* ``x[index] = y``
* ``x = y[index]``
* ``x.member = y``
* ``x = y.member``
* ``goto addr``
* ``if condition => addr``
* ``call result function arg1 arg2 ...``
* ``return value``
* ``new object source arg1 arg2 ...``
* ``list identifier element1 element2...``
* ``array identifier length``
* ``free identifier``
* ``pushscope``
* ``popscope``
* ``def identifier``
* ``pushcatch addr``
* ``popcatch``
* ``getexception identifier``
* ``sfn port arg``
* ``function identifier arg1 arg2 ...``
* ``class identifier member1 member2 * member3...``
* ``end``