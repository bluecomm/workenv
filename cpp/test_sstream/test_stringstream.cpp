#include <sstream>
#include <iostream>
#include <stdint.h>
#include <string>

bool integer2string(uint64_t num, std::string& out_str) {
    std::ostringstream sstream;
    sstream << num;
    out_str = sstream.str();
    return true;
}

bool string2integer2(const std::string& in_str, uint64_t& num) {
    std::stringstream sstream;
    sstream << in_str;
    sstream >> num;
    return true;
}

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

    uint64_t num;
    std::string out_str;

    integer2string(12345, out_str);
    std::cout << out_str << std::endl;
    
    string2integer2("a1234567a", num);
    std::cout << num << std::endl;

    return 0;
}
