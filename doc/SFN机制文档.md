# SFN机制文档

### 定义

SFN，全称Stamon For Native。是Stamon的一个调用外部功能的机制。你可以用它与解释器交互。

SFN的源码位于``src/sfn/SFN.cpp``。

SFN在Stamon中的语法规定为``sfn port, arg;``，其中port必须为字符串，代表着端口号，使用不同的端口号会调用不同的本地库，arg则是参数，具体类型由本地库的要求而定。在调用SFN后，arg可能会变为调用后的结果。

### 核心接口

SFN类的主要接口有：

* ``SFN(STMException *e, vm::ObjectManager *objman)``：构造函数objman为当前运行时的ObjectManager对象。
* ``void call(String port, datatype::Variable *arg)``：根据端口号调用本地库。

### 自定义外部功能

我们以实现一个输出功能为例。

##### 定义外部函数

想要自定义外部功能，需要先编写一个参数和返回值都符合规范的，位于全局作用域上的外部函数。由于参数列表过长，因此提供了如下宏：

```C++
#define SFN_PARA_LIST \
	stamon::sfn::SFN &sfn, stamon::datatype::Variable *arg, \
			stamon::vm::ObjectManager *manager

// 用这个宏（SFN Parameter List）可以快速声明SFN函数的参数列表
```

其中，sfn是调用此外部函数的sfn类，arg是参数变量，manager是对象管理器。

外部函数的返回值类型必须为``void``。因此，该输出功能的函数原型是：

```C++
void sfn_print(SFN_PARA_LIST);
```

该定义应作为前置定义放在``SFN``类的实现之前

##### 实现外部函数

外部函数可以使用移植接口上的函数，对于移植在平台上的开发者而言，也可以引入更多平台特有的函数进行操作。外部函数应该使用``stamon::exception::sfn::SFNError``作为异常生成函数，开发者也可以引入更多细化的异常生成函数。

``sfn_print``的实际实现如下：

```C++
void sfn_print(SFN_PARA_LIST) {

	using namespace stamon;

	STMException *ex = sfn.ex;
	datatype::DataType *val = arg->data;
	if (val->getType() != datatype::StringTypeID) {
		THROW(exception::sfn::SFNError(
				"sfn_print()", "invalid type"));
		return;
	}
	platform_print(((datatype::StringType *) val)->getVal());
	return;
}
```

该实现应该放在``SFN``类的实现之后

##### 绑定外部函数

实现完外部函数后，就要将其绑定在SFN上，以便调用时能找到并调用该外部函数。外部函数需要绑定在SFN的构造函数中。SFN的构造函数代码以如下为例：

```C++
SFN(STMException *e, vm::ObjectManager *objman) {
    ex = e;

    manager = objman;

    // 在这里将库函数按接口号填入
    sfn_functions.put(String("print"), sfn_print);
    sfn_functions.put(String("int"), sfn_int);
    sfn_functions.put(String("str"), sfn_str);
    //...略...
}
```

``sfn_functions``是一个Map，存储的值类型是符合外部函数原型的函数指针。在构造函数中加入利用``sfn_functions.put``来指定端口号及其对应的外部函数