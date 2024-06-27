//系统类

import stddef;
import math;

System = class {

    def version;

    func __init__(self) {
        self.version = "2.4.6";  //当前版本号
    }

    func typeof(self, v) {
        //返回类型
        def port = "typeof";
        def arg = v;
        sfn port, arg;
        return arg;
    }

    func throw(self, s) {
        //抛出异常，同时让虚拟机退出
        def port = "throw";
        def arg = str(s);
        sfn port, arg;
    }

    func system(self, s) {
        def port = "system";
        def arg = str(s);
        sfn port, arg;
        return arg;
    }

    func exit(self, n) {
        def port = "exit";
        def arg = int(n);
        sfn port, arg;
    }

}.new;