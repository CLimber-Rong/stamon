//系统类

import stddef;
import math;

Stamon = class {

    func typeof(self, v) {
        //给定对象，返回其类型
        return native("typeof", v);
    }

    func version(self) {
        //返回版本号
        return native("version", null);
    }
    
}.new;

System = class {

    func throw(self, s) {
        //抛出异常，同时让虚拟机退出
        return native("throw", s);
    }

    func system(self, s) {
        return native("system", s);
    }

    func exit(self, n) {
        native("exit", int(n));
    }

}.new;