#include <iostream>

int main() {
    int a = 1;
    int b = 3;
    double c = a / b;
    std::cout << "c: " << c << std::endl;
    double d = (double)a / b;
    std::cout << "d: " << d << std::endl;
    double e = (double)(a / b);
    std::cout << "e: " << e << std::endl;
    double f = (double)a / (double)(b);
    std::cout << "f: " << f << std::endl;
    return 0;
}
