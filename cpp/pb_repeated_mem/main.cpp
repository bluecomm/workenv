#include <unistd.h>
#include <iostream>
#include "test.pb.h"


void test0() {
    OuterMsg om;
    for (int i = 0; i < 1000; ++i) {
        om.add_in_1kb()->mutable_in_256_1()->mutable_in_64b_1()->set_u64_1(1024);
        std::cout << "count: " << i << std::endl;
        std::cout << sizeof(om) <<std::endl;
        std::cout << om.in_1kb_size() << std::endl;
        usleep(20000);
    }
}

void test1() {
    OuterMsg* om = new OuterMsg();
    for (int i = 0; i < 1000; ++i) {
        om->add_in_1kb()->mutable_in_256_1()->mutable_in_64b_1()->set_u64_1(1024);
        std::cout << "count: " << i << std::endl;
        std::cout << sizeof(*om) <<std::endl;
        std::cout << om->in_1kb_size() << std::endl;
        usleep(20000);
    }
    delete om;
}

int main() {
    std::cout << sizeof(OuterMsg) <<std::endl;
    test0();
    return 0;
    test1();
    sleep(10);
    test1();
    sleep(10);
    test1();
    return 0;
}

