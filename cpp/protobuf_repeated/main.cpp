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

void print_ns(const std::string& name, NewStyle& ns) {
    std::cout << name << std::endl;
    if (ns.has_planid()) {
        std::cout << "planid: " << ns.planid() << std::endl;
    } else {
        std::cout << "planid: null" << std::endl; 
    }
    if (ns.has_unitid()) {
        std::cout << "unitid: " << ns.unitid() << std::endl;
    } else {
        std::cout << "unitid: null" << std::endl; 
    }
    if (ns.has_cmatch()) {
        std::cout << "cmatch: " << ns.cmatch() << std::endl;
    } else {
        std::cout << "cmatch: null" << std::endl; 
    }

}
int main() {
    std::cout << "***********************************************************" << std::endl;
    /* 
     test optinal MEM use
    std::cout << sizeof(OuterMsg) <<std::endl;
    test0();
    return 0;
    test1();
    sleep(10);
    test1();
    sleep(10);
    test1();
    */
    std::cout << "***********************************************************" << std::endl;
    // test MergeFrom
    NewStyle ns1;
    NewStyle ns2;
    ns1.set_planid(11);
    ns1.set_unitid(12);
    ns2.set_unitid(22);
    ns2.set_cmatch(23);
    //std::cout << "ns1: " << ns1.ShortDebugString() << std::endl; 
    //std::cout << "ns2: " << ns2.ShortDebugString() << std::endl; 
    print_ns("ns1", ns1);
    print_ns("ns2", ns2);
    NewStyle ns3;
    ns3.CopyFrom(ns1);
    ns3.MergeFrom(ns2); // ns1.MergeFrom(ns2)
    //std::cout << "ns3: " << ns3.ShortDebugString() << std::endl; 
    print_ns("ns3: ns1.MergeFrom(ns2)", ns3);
    NewStyle ns4;
    ns4.CopyFrom(ns2);
    ns4.MergeFrom(ns1); // ns2.MergeFrom(ns1)
    //std::cout << "ns3: " << ns4.ShortDebugString() << std::endl; 
    print_ns("ns4: ns2.MergeFrom(ns1)", ns4);
    std::cout << "***********************************************************" << std::endl;
    return 0;
}

