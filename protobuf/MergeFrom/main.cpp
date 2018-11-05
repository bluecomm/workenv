#include <iostream>
#include "test.pb.h"

int main () {
    TestProto pa;
    TestProto pb;
    TestProto pc;
    TestProto pd;
    pa.set_str1("a1");
    pa.set_str2("a2");
    pa.set_str4("a4");

    pb.set_str3("b3");
    pb.set_str4("b4");

    std::cout << pa.ShortDebugString() << std::endl;
    std::cout << pb.ShortDebugString() << std::endl;

    pa.MergeFrom(pb);
    std::cout << pa.ShortDebugString() << std::endl;
    std::cout << pb.ShortDebugString() << std::endl;

    //pb.MergeFrom(pa);
    //std::cout << pa.ShortDebugString() << std::endl;
    //std::cout << pb.ShortDebugString() << std::endl;

    return 0;
}
