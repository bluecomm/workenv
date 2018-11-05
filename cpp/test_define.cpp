#include <iostream>

#define TEST_DEFINE(a, b) \
({ \
    int c = a + b; \
    c; \
})

char dict_name[] = "a";
int main() {
    std::cout << (TEST_DEFINE(1,2)) << std::endl;
    if (TEST_DEFINE(1,2) > 0) {
        std::cout << "ok" << std::endl;
    }
    std::cout << dict_name << std::endl;
    // *dict_name = "b";
    dict_name[0] = 'b';
    std::cout << dict_name << std::endl;
    return 0;
}
