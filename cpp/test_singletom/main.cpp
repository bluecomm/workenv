// https://www.cnblogs.com/myd620/p/6133420.html
#include <iostream>
#include <string>

#include "singlleton_a_hunger.h"
#include "singlleton_b_lazy.h"
#include "singlleton_c_lock.h"
#include "singlleton_d_check_lock.h"
#include "singlleton_e_check_lock_local.h"
#include "singlleton_f_pthread_once.h"
#include "singlleton_g_args.h"

struct A {
    A() : _a(4096), _b(8192) {};
    A(int a ,int b):_a(a),_b(b) {};
    int _a;
    int _b;
};

#define TEAT_SINGLETON(cla) \
{\
    struct A* aa1 = cla<A>::get_instance(); \
    struct A* aa2 = cla<A>::get_instance(); \
    std::cout << #cla << " : " << (aa1 == aa2) << " " << aa1->_a << " " << aa2->_a << " " << std::endl; \
}

int main() {
    TEAT_SINGLETON(SingletonA);
    TEAT_SINGLETON(SingletonB);
    TEAT_SINGLETON(SingletonC);
    TEAT_SINGLETON(SingletonD);
    TEAT_SINGLETON(SingletonE);
    TEAT_SINGLETON(SingletonF);

   int *p1 = SingletonG<int>::get_instance(5);
   int *p2 = SingletonG<int>::get_instance(10);
   std::cout << *p1 << "\t" << *p2 << std::endl;
   std::string *p3 = SingletonG<std::string>::get_instance("aa");
   std::string *p4 = SingletonG<std::string>::get_instance("bb");

   std::cout << *p3 << "\t" << *p4 << std::endl;

   A *p5 = SingletonG<A>::get_instance(1,2);

   A *p6 = SingletonG<A>::get_instance(4,5);

   std::cout << p5->_a << "\t" << p6->_a << std::endl;

   return 0;
}
