#pragma once

#include <pthread.h>

template <class T>
class SingletonE {
protected:
    SingletonE(){};
private:
    SingletonE(const SingletonE&){};
    SingletonE& operator=(const SingletonE&){};
    static T* m_instance;
    static pthread_mutex_t mutex;
public:
    static T* get_instance();
};

template <class T>
T* SingletonE<T>::get_instance() {
    if(m_instance == NULL) {
        pthread_mutex_lock(&mutex);
        if( m_instance == NULL) { 
             T* ptmp = new T();
             m_instance = ptmp;
        }
        pthread_mutex_unlock(&mutex);
    }
    return m_instance;
}

template <class T>
pthread_mutex_t SingletonE<T>::mutex = PTHREAD_MUTEX_INITIALIZER;

template <class T>
T* SingletonE<T>::m_instance = NULL;
