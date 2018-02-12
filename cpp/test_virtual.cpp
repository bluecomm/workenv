#include <iostream>

class Base {
public:
    virtual int test() = 0;
    virtual int test(int a);
};

int Base::test(int a) {
    std::cout << "I am in Base::test(int)" << std::endl;
    return test();
}

class DerivedA : public Base {
public:
    virtual int test();
};

int DerivedA::test() {
    std::cout << "I am in DerivedA::test()" << std::endl; 
    return 0;
}

class DerivedB : public Base {
public:
    virtual int test();
    virtual int test(int a);
};

int DerivedB::test() {
    std::cout << "I am in DerivedB::test()" << std::endl; 
    return 0;
}

int DerivedB::test(int a) {
    std::cout << "I am in DerivedB::test(int)" << std::endl; 
    return 0;
}

int main() {
    Base* base_ptr = NULL;
    DerivedA *da = new DerivedA();
    DerivedB *db = new DerivedB();

    base_ptr = da;
    base_ptr->test();
    base_ptr->test(0);

    base_ptr = db;
    base_ptr->test();
    base_ptr->test(0);
    return 0;
}
