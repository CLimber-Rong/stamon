# CITP设计文档

### 摘要

在C++中，前人已经提出了多种约束接口的方式。其中，CRTP不妨是一个适用于低标准且不需要性能的静态多态设计方式。但CRTP无法约束构造函数，实现过程涉及静态转换，代码复杂。为此，我们提出了CRTP的变式——**CITP**（**Curiously Iterating Template Pattern，奇异递推模板模式**），并将其首次应用到Stamon当中。

### 需求性分析

我们先来引入一段典型的CRTP设计：

```C++
/* 例1 */
#include <iostream>
using namespace std;

template<class Impl>
class Constraint {
	public:
	int square(int x) {
        // 被开发者调用的接口
		return static_cast<Impl*>(this)->square(x);
        // 静态转换为Implementation，并调用其实现，实现接口约束
	}
};

class Implementation : public Constraint<Implementation> {
	public:
	int square(int x) {
        // Constraint::f(int)的实现
		return x*x;
	}
};

using Square = Implementation;

int main()
{
  	Square s;
	cout<< s.square(5) <<endl;    // 运行结果：25
	return 0;
}
```

以上设计存在以下缺陷：

* 使用特殊的模板递归形式，学习成本高，难以理解
* 将this静态转换为子类并调用，实现复杂
* 无法安全地对构造函数进行约束

导致以上缺陷的原因如下：

* 用父类约束子类，又将子类作为类型参数传递给父类，产生奇异递归
* 在父类中调用子类的成员，需要进行静态转换
* 构造函数一般自底而上调用，因此父类无法简便地约束构造函数

在经过大量地实验性设计后，我们综合了代理模式等设计模式，提出了一种可以解决以上缺陷的方案：与CRTP将用于接口约束的类作为父类的做法相反，我们将用于接口约束的类作为子类，受约束的类作为父类，从而让接口约束从递归变为递推。

这种方案具有以下优点：

* 不需要复杂的递归形式
* 让受约束的父类被子类调用，鉴于子类可以安全调用父类成员，用于约束的类无需静态转换
* 用于约束的子类在构造时可以将构造函数完美转发给受约束的父类

综上所述，在某些情景下，CITP能比CRTP约束更多东西，且能减少思维量。

### 设计

我们来看看 _例1_ 在CITP中的设计：

```C++
/* 例2 */
#include <iostream>
using namespace std;

class Implementation {
	// 实现类作为父类受约束
	public:
	int square(int x) {
		// 受约束接口
		return x*x;
	}
};

template<class Impl>
class Constraint : public Impl {
	// 用于约束的类作为子类，继承实现类
	public:
	int square(int x) {
		// 调用Impl的实现
		return Impl::square(x);
	}
};

using Square = Constraint<Implementation>;
// 将Implementation传递给Constraint作为约束

int main()
{
  	Square s;
	cout<< s.square(5) <<endl;    // 运行结果：25
	return 0;
}
```

在 _例2_ 中，我们将Constraint作为一个模板类，并使其继承模板提供的类，因此Constraint可以完美的调用其父类（即受约束的类）的成员，包括构造函数。开发者在实现时应使用using关键字给Constraint<Implementation>赋予供用户使用的别名（例如 _例2_ 的Square）。

我们再看看在CITP中，如何约束构造函数：

```C++
/* 例3 */
#include <iostream>
using namespace std;

class Implementation {
	// 实现类作为父类受约束
	int k, b;
	public:
	Implementation(int K, int B) : k(K), b(B) {};
	int f(int x) {
		// 受约束接口
		return k*x + b;
	}
};

template<class Impl>
class Constraint : public Impl {
	// 用于约束的类作为子类，继承实现类
	public:
	Constraint(int K, int B) : Impl(K,B) {};
	int f(int x) {
		// 调用Impl的实现
		return Impl::f(x);
	}
};

using Linear = Constraint<Implementation>;
// 将Implementation传递给Constraint作为约束

int main()
{
  	Linear l(4,5);
	cout<< l.f(5) <<endl;    // 运行结果：25
	return 0;
}
```

可以看到，Constraint只需要正常编写构造函数，并转发给父类的构造函数，即可实现构造函数的约束。此外，由于析构函数的调用顺序是从子类到父类，因此Constraint哪怕没有编写析构函数，Implementation的析构函数也照样会被正常调用。但这也使得析构函数无法收到约束——CITP难以保证受约束的类一定拥有开发者手写的析构函数。

CITP的设计中，约束重载运算符的方式和约束普通方法的方式是不同的。我们将 _例3_ 中的函数``int f(int)``改装成``int operator<<(int)``，来看看CITP是如何约束重载运算符的：

```C++
/* 例4 */
#include <iostream>
using namespace std;

class Implementation {
	// 实现类作为父类受约束
	int k, b;
	public:
	Implementation(int K, int B) : k(K), b(B) {};
	int operator<<(int x) {
		// 受约束接口
		return k*x + b;
	}
};

template<class Impl>
class Constraint : public Impl {
	// 用于约束的类作为子类，继承实现类
	public:
	Constraint(int K, int B) : Impl(K,B) {};
	int operator<<(int x) {
		// 调用Impl的实现
		return Impl::operator<<(x);
	}
};

using Linear = Constraint<Implementation>;
// 将Implementation传递给Constraint作为约束

int main()
{
  	Linear l(4,5);
	cout<< (l<<5) <<endl;    // 运行结果：25
	return 0;
}
```

### 结论

CITP相较于CRTP，更专注于接口约束，是一个适用于低C++标准的高性能方案。但CITP目前仍然存在缺陷：

* 无法具体约束函数的行为，也无法约束析构函数
* 在传递参数、返回值时也可能发生参数、返回值的隐式转换且无法约束这种转换
* 如果CITP模板代码处于未使用状态，编译器可能不会进行接口约束检查

这些问题正在研究当中，有待解决。