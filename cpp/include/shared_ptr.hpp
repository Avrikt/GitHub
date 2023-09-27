/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Adir Ortal   				    *
* Date: 03.08.2023						    	*
* Description: Header file for Shared Pointer  	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/

#ifndef MY_PROJECT_SHARED_PTR_HPP
#define MY_PROJECT_SHARED_PTR_HPP

#include <cstddef>  //size_t
#include <new>      //operator new

namespace dev
{

template <typename T>
class SharedPtr 
{
public:
    template<typename Y>
    friend class SharedPtr;
    
    template< typename Y > 
     SharedPtr(Y* ptr = nullptr);
    template< typename Y >
    SharedPtr(SharedPtr<Y>& other_);
    ~SharedPtr();
    template< typename Y >
    SharedPtr<T>& operator=(SharedPtr<Y>& other_);

    const T& operator*() const;
    T* operator->() const;
    T* Get() const;

private:
    T* m_data;
    size_t* m_ref_count;
    void *operator new(size_t size);
};

/************************************ Function Definitions *******************************************/
template <typename T>
template <typename Y>
SharedPtr<T>::SharedPtr(Y* ptr) : m_data(ptr), m_ref_count(new size_t(1UL)) {}

template <typename T>
template <typename Y>
SharedPtr<T>::SharedPtr(SharedPtr<Y>& other_): m_data(other_.m_data), m_ref_count(other_.m_ref_count)
{
    ++(*m_ref_count);
}

template <typename T>
template <typename Y>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<Y> &other_)
{
    ++(*other_.m_ref_count);
    
    if(*m_ref_count == 1)
    {
        this->~SharedPtr();
    }
    else
    {
        --(*m_ref_count);
    }
    m_data = other_.m_data;
    m_ref_count = other_.m_ref_count;

    return *this;
} 

template <typename T>
SharedPtr<T>::~SharedPtr()
{
    if (0 == --(*m_ref_count))
    {
        delete m_ref_count;
        delete m_data;
    }
}

/************************************ Operator Overloading Functions *******************************************/
template <typename T>
const T& SharedPtr<T>::operator*() const {return *m_data;}

template <typename T>
T* SharedPtr<T>::operator->() const {return m_data;}

/************************************ Member Functions *********************************************************/ 
template <typename T>
T* SharedPtr<T>::Get() const {return m_data;}

}   //namespace dev

#endif //MY_PROJECT_SHARED_POINTER