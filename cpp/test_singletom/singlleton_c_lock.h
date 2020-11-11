#pragma once

#include <pthread.h>

template <class T>
class SingletonC {
protected:
    SingletonC(){};
private:
    SingletonC(const SingletonC&){};
    SingletonC& operator=(const SingletonC&){};
    static T* m_instance;
    static pthread_mutex_t mutex;
public:
    static T* get_instance();
};

template <class T>
T* SingletonC<T>::get_instance() {
    pthread_mutex_lock(&mutex);
    if( m_instance == NULL) { 
        m_instance = new T();
    }
    pthread_mutex_unlock(&mutex);
    return m_instance;
}

template <class T>
pthread_mutex_t SingletonC<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <class T>
T* SingletonC<T>::m_instance = NULL;
