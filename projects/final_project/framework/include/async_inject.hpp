/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 14.09.2023								*
* Description: Header file for Async_inject		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_ASYNC_INJECT_HPP
#define MY_PROJECT_ASYNC_INJECT_HPP

#include "singleton.hpp"
#include "scheduler.hpp"

namespace dev
{

class AsyncInjection : public Scheduler::ITask
{
public:
    typedef std::function<bool()> ActionFunc;
    
    static void Create(const ActionFunc &proc, time_t time_interval);
    virtual void Execute();

private:
    AsyncInjection(const ActionFunc &proc, time_t time_interval);
    ~AsyncInjection() = default;

    ActionFunc m_func;
    time_t m_timeInterval;
};

} //namespace dev

#endif //MY_PROJECT_ASYNC_INJECT_HPP

