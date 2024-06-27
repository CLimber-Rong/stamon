//标准输入输出库

import stddef;
import math;
import string;

func puts(s) {
    def port = "puts";
    sfn port, s;
}

func print(n) {
    puts(str(n));
}

func println(n) {
    puts(str(n));
    puts("\n");
}

func input() {

    def port = "input";
    def arg;

    sfn port, arg;

    return arg;
}