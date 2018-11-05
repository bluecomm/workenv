#include <iostream>

class A {
public:
    A() {};
    virtual void f1() const = 0;

private:
    void f2() {
        std::cout << "I am in A::f2()" << std::endl;
    };
};

class B : virtual public A {
public:
    virtual void f1() const;
};

void B::f1() const {
    std::cout << "I am in B::f1()" << std::endl;
}
/* bad
void B::f1() {
    std::cout << "I am in B::f1()" << std::endl;
}
*/

int main() {
    A* aa = new B();
    aa->f1();

    B aaa;
    aaa.f1();

    return 0;
}
