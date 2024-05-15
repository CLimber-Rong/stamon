



import std; //支持调用文件

def a = 1+1, b = a*2;   //定义变量

if a == b {
    puts("a==b!\n");
} else {
    puts("a!=b!\n");
}

//如果代码块只有一条语句，那么可以只用冒号来编写

def i = 1;

while i<=3 {
    puts("This is a while loop!\n");
    i += 1;
}

for j in { 1, 2, 3 } {  //for循环的表达式要是列表
    println(j);
}

def a = [3];   //默认填充null
println(a);

func f1() {  //如果函数无参数，也可以把括号省略掉，写成“func f {...}”格式
    puts("This is function \"f\"!\n");
}

def f2 = func(n) {
    println(n);
};

f1();
f2(10);

class c1 {
    def member;
    func __init__(self) {    //构造函数名默认__init__，并且所有方法都需要有self参数
        puts("Initializing c1!\n");
        self.member = 0;    //用self访问成员
    }
}

def c2 = class {
    func __init__(self, n) {
        puts("Initializing c2!\n");
        println(n);
    }
};

c1.new;
c2.new(8);