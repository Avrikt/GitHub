/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 14.09.2023								*
* Description: Source file for Async_inject			*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "async_inject.hpp"
#include "utilities.hpp"

/************************************Functions***************************************************/

namespace dev
{

AsyncInjection::AsyncInjection(const ActionFunc &proc, time_t time_interval) : m_func(proc), m_timeInterval(time_interval) {}

void AsyncInjection::Create(const ActionFunc &proc, time_t time_interval)
{
    Scheduler *sched = Singleton<Scheduler>::GetInstance();
    AsyncInjection *ptr = new AsyncInjection(proc, time_interval);
    std::shared_ptr<AsyncInjection> shared_ptr (ptr, [](AsyncInjection *ptr){delete ptr;});
    sched->AddTask(shared_ptr, time_interval);
}

void AsyncInjection::Execute()
{
    if(nullptr != m_func && false == m_func())
    {
        Create(m_func, m_timeInterval);
    }
}

} //namespace dev
