#include <time.h>
#include <stdint.h>
#include <iostream>
 #include <typeinfo>

class A {
public:
    A() : m_a(0), m_b(0) , _ma(0), _mb(0) {}
    virtual ~A() {};
    uint32_t m_a;
    uint32_t m_b;

    void set_ma(uint32_t ma) {
        _ma = ma;
    }
    void set_mb(uint32_t mb) {
        _mb = mb;
    }
    uint32_t get_ma() {
        return _ma;
    }
    uint32_t get_mb() {
        return _mb;
    }

private:
    uint32_t _ma;
    uint32_t _mb;
};

class B {
public:
    B() : m_a(0), m_b(0), _ma(0), _mb(0) {}
    virtual ~B() {};
    uint32_t m_a;
    uint32_t m_b;

    void set_ma(uint32_t ma) {
        _ma = ma;
    }
    void set_mb(uint32_t mb) {
        _mb = mb;
    }
    uint32_t get_ma() {
        return _ma;
    }
    uint32_t get_mb() {
        return _mb;
    }

private:
    uint32_t _ma;
    uint32_t _mb;
};

template <typename T>
void print_values(T* temp) {
    std::cout << typeid(T).name() << ": " << temp->m_a << " " << temp->m_b << " " << 
        temp->get_ma() << " " << temp->get_mb() << std::endl;
}

template <typename T>
class BaseSharder {
public:
    BaseSharder();
    virtual ~BaseSharder();
    bool init();
    void run();
private:
    T* _m_temp;
};

// the impletion of template class must be in header file
template <typename T>
BaseSharder<T>::BaseSharder() : _m_temp(NULL) {}

template <typename T>
BaseSharder<T>::~BaseSharder(){
}

template <typename T>
bool BaseSharder<T>::init() {
    if (_m_temp != NULL) {
        std::cout << "already inited" << std::endl;
        return true;;
    }
    _m_temp = new T();   
    if (_m_temp == NULL) {
        std::cout << "init failed" << std::endl;
        return false;
    }
    time_t tm = time(NULL);
    _m_temp->m_a = tm % 3;
    _m_temp->m_b = tm % 4;
    _m_temp->set_ma(tm % 5);
    _m_temp->set_mb( tm % 6);
    std::cout << "init success" << std::endl;
    return true;
}

template <typename T>
void BaseSharder<T>::run() {
    print_values<T>(_m_temp);
}

/*
template <typename T>
class BaseSharder {
public:
    BaseSharder() : _m_temp(NULL) {}
    bool init() {
        if (_m_temp != NULL) {
            std::cout << "already inited" << std::endl;
            return true;;
        }
        _m_temp = new T();   
        if (_m_temp == NULL) {
            std::cout << "init failed" << std::endl;
            return false;
        }
        time_t tm = time(NULL);
        _m_temp->m_a = tm % 3;
        _m_temp->m_b = tm % 4;
        _m_temp->set_ma(tm % 5);
        _m_temp->set_mb( tm % 6);
        std::cout << "init success" << std::endl;
        return true;
    }

    void run() {
        print_values<T>(_m_temp);
    }
private:
    T* _m_temp;
};
*/

class MySharder : public BaseSharder<A> {
};
