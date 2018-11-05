#include <iostream>

typedef int (*myfunc)(int);

int func_null(int in) {
    std::cout << "I am in func null : " << in << std::endl;
    return 0;
}

int func_a(int in) {
    std::cout << "I am in func a : " << in << std::endl;
    return 0;
}

int func_b(int in) {
    std::cout << "I am in func b : " << in << std::endl;
    return 0;
}

void set_funa(myfunc mf) {
    std::cout << "set_funa" << std::endl;
    mf = func_a;
}

void set_funb(myfunc mf) {
    std::cout << "set_funb" << std::endl;
    mf = func_b;
}

int main() {
    myfunc mf = func_null;
    mf(1);
    set_funa(mf);
    mf(1);
    set_funb(mf);
    mf(1);
    return 0;
}
