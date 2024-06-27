import stddef;

func str(s) {
    def port = "str";
    def arg = s;
    sfn port, arg;
    return arg;
}