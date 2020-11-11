#pragma once

#include <pthread.h>

template <class T>
class SingletonF {
protected:
    SingletonF(){};
private:
    SingletonF(const SingletonF&){};
    SingletonF& operator=(const SingletonF&){};
    static T* m_instance;
    static pthread_once_t m_once;
public:
    static void Init();
    static T* get_instance();
};

template <class T>
void SingletonF<T>::Init() {
    m_instance = new T();
}

template <class T>
T* SingletonF<T>::get_instance() {
    pthread_once(&m_once,Init);
    return m_instance;
}

template <class T>
pthread_once_t SingletonF<T>::m_once = PTHREAD_ONCE_INIT;

template <class T>
T* SingletonF<T>::m_instance = NULL;
