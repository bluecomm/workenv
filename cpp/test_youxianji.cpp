#include <iostream>

int main () {
    if (0x01 & 0x03 == 0x01) {
        std::cout << "1" << std::endl;
    } else {
        std::cout << "2" << std::endl;
    }
    if ((0x01 & 0x03) == 0x01) {
        std::cout << "3" << std::endl;
    } else {
        std::cout << "4" << std::endl;
    }
    return 0;
}
