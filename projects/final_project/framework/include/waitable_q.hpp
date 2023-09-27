/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai  				    *
* Date: 13.08.2023						    	*
* Description: Header file for Waitable Queue  	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#ifndef MY_PROJECT_WAITABLE_Q_HPP
#define MY_PROJECT_WAITABLE_Q_HPP

#include <iostream>                         // std::cerr
#include <boost/noncopyable.hpp>            // boost::noncopyable
#include <boost/chrono.hpp>                 // boost::chrono
#include <boost/thread/mutex.hpp>           // boost::mutex
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

namespace dev
{
template<typename Container>
class WaitableQueue : private boost::noncopyable
{
public:
    typedef typename Container::value_type val_type;
    typedef typename Container::reference ref_val_type;

    WaitableQueue();

    void Push(const val_type& value);
    void Pop(ref_val_type ref);
    bool PopTimeout(ref_val_type ref, boost::chrono::milliseconds ms);
    bool IsEmpty() const;

private:
    Container m_pq;
    boost::interprocess::interprocess_semaphore m_sem;
    mutable boost::mutex m_mutex;
};

template<typename Container>
WaitableQueue<Container>::WaitableQueue(): m_sem(0) {}

template<typename Container>
void WaitableQueue<Container>::Push(const val_type& value)
{
    std::lock_guard<boost::mutex> lock(m_mutex);
    m_pq.push(value);
    m_sem.post();
}

template <typename Container>
void WaitableQueue<Container>::Pop(ref_val_type ref)
{
    m_sem.wait();
    std::lock_guard<boost::mutex> lock(m_mutex);
    ref = m_pq.front();
    m_pq.pop();
}

template <typename Container>
bool WaitableQueue<Container>::PopTimeout(ref_val_type ref, boost::chrono::milliseconds ms)
{
    using namespace boost::posix_time;

    if(!m_sem.timed_wait(second_clock::universal_time() + milliseconds(ms.count())))
    {
        std::cerr << "Timeout\n";
        return false;
    }
    
    std::lock_guard<boost::mutex> lock(m_mutex);
    ref = m_pq.front();
    m_pq.pop();
    return true;
}

template <typename Container>
bool WaitableQueue<Container>::IsEmpty() const
{
    std::lock_guard<boost::mutex> lock(m_mutex);
    return (m_pq.empty());
}
} //namespace dev

#endif //MY_PROJECT_WAITABLE_Q_HPP