#pragma once

template <class T>
class SingletonA {
protected:
    SingletonA() {};
private:
    SingletonA(const SingletonA&) {};
    SingletonA& oparator=(const SingletonA&) {};
    static T* m_instance;
public:
    static T* get_instance();
};

template <class T>
T* SingletonA<T>::get_instance() {
    return m_instance;
}

template <class T>
T* SingletonA<T>::m_instance = new T();
