#pragma once

template <class T>
class SingletonB {
protected:
    SingletonB(){};
private:
    SingletonB(const SingletonB&){};
    SingletonB& operator=(const SingletonB&){};
    static T* m_instance;
public:
    static T* get_instance();
};

template <class T>
T* SingletonB<T>::get_instance() {
    if( m_instance == NULL)
    { 
        m_instance = new T();
    }
    return m_instance;
}

template <class T>
T* SingletonB<T>::m_instance = NULL;
