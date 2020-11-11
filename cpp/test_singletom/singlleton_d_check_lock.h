#pragma once

#include <pthread.h>

template <class T>
class SingletonD {
protected:
    SingletonD(){};
private:
    SingletonD(const SingletonD&){};
    SingletonD& operator=(const SingletonD&){};
    static T* m_instance;
    static pthread_mutex_t mutex;
public:
    static T* get_instance();
};

template <class T>
T* SingletonD<T>::get_instance() {
    if( m_instance == NULL) {
        pthread_mutex_lock(&mutex);
        if( m_instance == NULL) { 
             m_instance = new T();
        }
        pthread_mutex_unlock(&mutex);
    }
    return m_instance;
}

template <class T>
pthread_mutex_t SingletonD<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <class T>
T* SingletonD<T>::m_instance = NULL;
