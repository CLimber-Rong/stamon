# Stamon错误及警告信息修复指南

> 本文件为``src/exception/codegen/codegen.py``根据配置文件自动生成，用户可以用搜索文本的方式快速定位自己的问题

### astrunner

##### TypeError

错误的数据类型被参与计算。例如字符串与整数相加时会触发此错误。

##### PostfixError

未知后缀类型。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

##### IndexError

数列访问越界。当访问的下标大等于数列长度时会触发此错误。

##### ConstantsError

错误的常量表下标。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

##### ZeroDivisionError

零作为除数。请检查运算过程中是否出现零作为除数的现象。

##### BreakError

break语句在循环体外。break语句作为循环流程控制相关的语句，必须在循环语句之内出现。原则上该报错会在编译端出现，因此如果运行端才出现此报错，请检查编译端是否正常，以及传递给运行端的二进制文件是否合理。

##### ContinueError

continue语句在循环体外。continue语句作为循环流程控制相关的语句，必须在循环语句之内出现。原则上该报错会在编译端出现，因此如果运行端才出现此报错，请检查编译端是否正常，以及传递给运行端的二进制文件是否合理。

##### ArgumentsError

调用该函数需要的参数与所传递的参数个数不符。请检查调用函数的过程中是否出现参数传递错误。

##### ReturnError

return语句在函数外。return语句作为函数相关的语句，必须在函数体之内出现。

##### UnknownOperatorError

未知运算符。这是运行端内部错误，请检查传递给运行端的二进制文件是否合理。

##### UnknownMemberError

对象没有指定的成员，请检查成员标识符是否输入正确。

##### LengthError

长度必须是非负数。请检查申请数列的长度是否是负数。

##### NegativeShiftError

负数位移。位移运算中的位移数必须是非负数，请检查位移运算中的位移数是否是负数。

### compiler

##### StringError

字符串格式有误。例如转义字符输入错误时有可能触发此错误。请检查字符串是否由一对双引号包裹，转义字符是否输入正确。

##### FloatError

字符串格式有误。例如转义字符输入错误时有可能触发此错误。请检查字符串是否由一对双引号包裹，转义字符是否输入正确。

##### TokenError

未知的词法单元。当用户输入非法字符（例如英文问号）时会触发此错误，请检查代码中是否包含非法字符。

##### SyntaxError

综合语法错误。该报错的触发条件多种多样，在Stamon的语法当中，会出现一些固定的语法搭配，当用户输入的语法搭配与规定的不一致时，会触发此错误，例如语句末尾未加分号。请用户根据编译器报告的错误位置，对照《Stamon语法教程手册》，检查语法搭配是否一致。值得注意的是：有些错误实际需要解决的位置，或许和编译器报告的错误位置并不完全一样，但两者肯定存在对应关系，所以请用户认真检查错误位置所在行及其关联的代码是否有误。

##### VariableRedeclaredError

变量被重复强定义。对于此类错误，推荐一种解决方案——前置定义设计：先为专门建立一个源码文件（被称为前置定义文件），并使用def关键字来强定义所有全局标识符（即前置定义）；并在其他源文件的开头先引用这个前置定义文件，在其他源文件中，不直接使用def关键字来强定义全局标识符，而是采用直接赋值或弱定义的形式。这种代码设计思路，可以确保所有全局标识符只被强定义一次。对于局部标识符，请确保最多被强定义一次。有关如何使用强定义、弱定义和赋值，也可以参照《Stamon语法教程手册》中的相关内容。值得注意的是：请确保需要用到的全局标识符在def之后有相关的实现，否则在使用被强定义但未赋值的标识符时，会出现对空值进行操作的情况，从而引起运行时错误。

##### SfnError

SFN语句的端口或参数必须是标识符。

##### BreakError

break语句在循环体外。break语句作为循环流程控制相关的语句，必须在循环语句之内出现。

##### ContinueError

continue语句在循环体外。continue语句作为循环流程控制相关的语句，必须在循环语句之内出现。

##### MemberError

类成员名必须是标识符。

##### VariableError

变量名必须是标识符。

##### BraceError

花括号未闭合，请检查错误位置及其关联位置是否存在未闭合的花括号对。

##### SquareBracketError

方括号未闭合，请检查错误位置及其关联位置是否存在未闭合的方括号对。

##### RoundBracketError

圆括号未闭合，请检查错误位置及其关联位置是否存在未闭合的圆括号对。

##### ClassDefinedError

类定义中，只能定义成员函数、成员类和成员变量。

##### ImportError

不能引用其他源码。只有当编译器被指定为不能引用其他文件的模式，并且源码中调用了其他文件时，才会触发此错误。

##### AssignmentError

表达式的左操作数必须是左值。其中对左值的严格定义是：一个变量取任意次元素和成员的表达式。

##### UndefinedVariableError

未定义变量。请在使用变量前先使用强定义或弱定义的方式定义变量。

##### LargeIntegerWarning

整数过大从而导致溢出。这会导致程序执行时出现数值与代码中数值不同的情况。

##### LargeFloatWarning

浮点数过大从而导致溢出。这会导致程序执行时出现数值与代码中数值不同的情况。

### astfilereader

##### FormatError

Ast文件综合编码错误，请检查编码格式或重新编码。

##### NodeError

Ast文件出现了未知节点，请检查编码格式或重新编码。

##### RedundantRootNodeError

Ast文件有多余的根节点，请检查编码格式或重新编码。

##### RootNodeError

Ast文件没有根节点，请检查编码格式或重新编码。

##### RedundantEndNodeError

Ast文件有多余的结束单元，请检查编码格式或重新编码。

##### EndNodeError

Ast文件缺失结尾单元，请检查编码格式或重新编码。

### astir

##### RootNodeError

未在解码过程中发现根节点，请检查编码文件或重新编译。

##### RedundantEndNodeError

解码过程中出现多余的结束单元，请检查编码文件或重新编译。

##### RedundantRootNodeError

解码过程中出现多个根节点，请检查编码文件或重新编译。

##### EndNodeError

解码过程中缺失结束单元，请检查编码文件或重新编译。

### astirreader

##### CodeSizeError

AST-IR文件过小，请检查编码文件或重新编译。

##### FormatError

该文件的魔数不是0xABDB，由此推断该文件不是AST-IR字节码，请检查编码文件或重新编译。

##### ConstantsError

该文件存在未知的常量，请检查编码文件或重新编译。

### objectmanager

##### MemoryError

GC后占用内存仍然超出限制，请调整内存大小限制或优化代码。

##### PhysicalMemoryError

物理内存不足，请调整内存大小限制或优化代码。

##### IdentifierError

访问了未定义的标识符，通常情况下，内部标识符的定义问题会在编译期报告，因此需仔细检查外部标识符是否只定义未赋值，或字节码文件是否被损坏，或是否存在其他内部异常。

##### ObjectError

发现了未知类型的对象，通常情况下，对象由虚拟机生成，也可能由SFN等外部接入的代码产生，因此请仔细检查虚拟机是否存在内部异常或外部接入的代码是否存在异常。

### tokenfilereader

##### FormatError

文件格式综合错误，请检查编码文件或重新编码。

##### TokenError

该文件出现未知的Token，请检查编码文件或重新编码。

### sfn

##### SFNError

SFN产生的综合性错误。

##### SFNWarning

SFN产生的综合性警告。

