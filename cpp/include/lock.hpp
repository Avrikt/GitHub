/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Adir Ortal   				    *
* Date: 02.08.2023						    	*
* Description: Header file for Lock          	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#ifndef MY_PROJECT_LOCK_HPP
#define MY_PROJECT_LOCK_HPP

namespace dev
{

template < class T>
class LockGuard
{
public: 
    explicit LockGuard(T &lock_);
    ~LockGuard();

private:
    T &m_lock;
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

template <class T>
LockGuard<T>::LockGuard(T &lock_): m_lock(lock_) 
{
    m_lock.lock();
}

template <class T>
LockGuard<T>::~LockGuard()
{
    m_lock.unlock();
}

void RunLock(void);

} //namespace dev

#endif // MY_PROJECT_LOCK_HPP