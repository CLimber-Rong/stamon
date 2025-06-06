# Stamon错误及警告信息修复指南

用户在编写代码时，若出现编译期或运行时错误（或警告），本指南就可以提供更详细的解释信息以及修复方向。

## 编译端有关信息

### 错误信息

#### the string was entered incorrectly

字符串格式有误。例如转义字符输入错误时有可能触发此错误。请检查字符串是否由一对双引号包裹，转义字符是否输入正确。

#### the floating point was entered incorrectly

浮点数输入错误。当小数点后没有跟着数字时，会触发此错误，请检查浮点数的输入格式是否正确。

#### unknown token: 'XXX'

未知的词法单元。当用户输入非法字符（例如英文问号）时会触发此错误，请检查代码中是否包含非法字符。

#### invalid syntax

综合语法错误。该报错的触发条件多种多样，在Stamon的语法当中，会出现一些固定的语法搭配，当用户输入的语法搭配与规定的不一致时，会触发此错误，例如语句末尾未加分号。请用户根据编译器报告的错误位置，对照《Stamon语法教程手册》，检查语法搭配是否一致。值得注意的是：有些错误实际需要解决的位置，或许和编译器报告的错误位置并不完全一样，但两者肯定存在对应关系，所以请用户认真检查错误位置所在行及其关联的代码是否有误。

#### variable "XXX" are declared repeatedly

变量被重复强定义。关于“强定义”的解释可以参照《Stamon语法教程手册》中的相关内容。对于此类错误，推荐一种解决方案——**前置定义设计**：
先为专门建立一个源码文件（被称为前置定义文件），并使用**def**关键字来强定义所有全局标识符（即前置定义）；并在其他源文件的开头先引用这个前置定义文件，在其他源文件中，不直接使用**def**关键字来强定义全局标识符，而是采用直接赋值或弱定义的形式。这种代码设计思路，可以确保所有全局标识符只被强定义一次。对于局部标识符，请确保最多被强定义一次。有关如何使用强定义、弱定义和赋值，也可以参照《Stamon语法教程手册》中的相关内容。值得注意的是：请确保需要用到的全局标识符在**def**之后有相关的实现，否则在使用被强定义但未赋值的标识符时，会出现对空值进行操作的情况，从而引起运行时错误。

#### the port or argument of the SFN statement must be an identifier

SFN语句的端口或参数必须是标识符。有关“SFN语句”的解释可以参照《Stamon语法教程手册》中的相关内容。

#### 'continue' outside loop

continue语句在循环体外。continue语句作为循环流程控制相关的语句，必须在循环语句之内出现。

#### 'break' outside loop

break语句在循环体外。break语句作为循环流程控制相关的语句，必须在循环语句之内出现。

#### the member name must be an identifier

类成员名必须是标识符。有关标识符的命名规范可以参照《Stamon语法教程手册》中的相关内容。

#### the name of the variable must be an identifier

变量名必须是标识符。有关标识符的命名规范可以参照《Stamon语法教程手册》中的相关内容。

#### the brace are not closed

花括号未闭合，请检查错误位置及其关联位置是否存在未闭合的花括号对。

#### the square bracket are not closed

方括号未闭合，请检查错误位置及其关联位置是否存在未闭合的方括号对。

#### the round bracket are not closed

圆括号未闭合，请检查错误位置及其关联位置是否存在未闭合的圆括号对。

#### only functions, classes and variables can be defined in a class

类定义中，只能定义成员函数、成员类和成员变量。有关类定义的规范可以参照《Stamon语法教程手册》中的相关内容。

#### cannot import

不能引用其他源码。只有当编译器被指定为不能引用其他文件的模式，并且源码中调用了其他文件时，才会触发此错误。有关参数指定的内容可以参照《Stamon发行版使用指南》。

#### the left operand of an assignment must be an lvalue

表达式的左操作数必须是左值。引用《Stamon语法教程手册》中的注意事项：“请确保赋值的左操作数是左值，其中对左值的严格定义是：一个变量取任意次元素和成员的表达式”。

#### undefined variable: "XXX"

未定义变量。请在使用变量前先使用强定义或弱定义的方式定义变量。

### 警告信息

#### the integer is too large

整数过大从而导致溢出。这会导致程序执行时出现数值与代码中数值不同的情况。

#### the floating point is too large

浮点数过大从而导致溢出。这会导致程序执行时出现数值与代码中数值不同的情况。

## 运行端（AstRunner）有关信息

### 错误信息

#### an error of data type 'XXX' occurred in the calculation

错误的数据类型被参与计算。例如字符串与整数相加时会触发此错误。

#### unknown type of postfix

未知后缀类型。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

#### list index out of range

数列访问越界。当访问的下标大等于数列长度时会触发此错误。

#### wrong index of constants

错误的常量表下标。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

#### division by zero

零作为除数。请检查运算过程中是否出现零作为除数的现象。

#### 'continue' outside loop

continue语句在循环体外。continue语句作为循环流程控制相关的语句，必须在循环语句之内出现。原则上该报错会在编译端出现，因此如果运行端才出现此报错，请检查编译端是否正常，以及传递给运行端的二进制文件是否合理。

#### 'break' outside loop

break语句在循环体外。break语句作为循环流程控制相关的语句，必须在循环语句之内出现。原则上该报错会在编译端出现，因此如果运行端才出现此报错，请检查编译端是否正常，以及传递给运行端的二进制文件是否合理。

#### takes A form arguments but B was given

调用该函数需要A个参数，但传递了B个参数。请检查调用函数的过程中是否出现参数传递错误。

#### 'return' outside function

return语句在函数外。return语句作为函数相关的语句，必须在函数体之内出现。

#### unknown operator

未知运算符。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

#### object has no member 'XXX'

对象没有名为XXX的成员，请检查成员标识符是否输入正确

#### the length must be non-negative

长度必须是非负数。请检查申请数列的长度是否是负数。

#### negative shift count

负数位移。位移运算中的位移数必须是非负数，请检查位移运算中的位移数是否是负数。

### 警告信息

暂无。