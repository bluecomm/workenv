#pragma once

#include <utility>

template <class T>
class SingletonG {
protected:
    SingletonG(){};
private:
    SingletonG(const SingletonG&){};
    SingletonG& operator=(const SingletonG&){};
    static T* m_instance;
public:
    template <typename... Args>
    static T* get_instance(Args&&... args) {
        if(m_instance == NULL)
            m_instance = new T(std::forward<Args>(args)...);
        return m_instance;
    }


    static void destroy_instance() {
        if(m_instance )
            delete m_instance;
        m_instance = NULL;
    }
};

template <class T>
T* SingletonG<T>::m_instance = NULL;
