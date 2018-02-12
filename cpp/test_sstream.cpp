#include <sstream>
#include <iostream>
#include <string>

int main() {
    std::stringstream ss;
    int a = 123;
    std::string b;
    ss << a;
    ss >> b;
    std::cout << b << std::endl;
    ss.clear();
    int c;
    ss << b;
    ss >> c;
    std::cout << c << std::endl;
    return 0;
}
