/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Dave Hairapetian					*
* Date: 13.09.2023								*
* Description: Source file for Scheduler		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "scheduler.hpp"
#include "utilities.hpp"

/************************************Functions***************************************************/

namespace dev
{

Scheduler::Scheduler() : m_curr_task(nullptr)
{
    struct sigevent sigev;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    sigev.sigev_notify = SIGEV_THREAD;
    sigev.sigev_notify_function = OnTimer;
    sigev.sigev_notify_attributes = &attr;
    sigev.sigev_value.sival_ptr = this;

    timer_create(CLOCK_REALTIME, &sigev, &m_timerid);
    pthread_attr_destroy(&attr);
}

Scheduler::~Scheduler()
{
    timer_delete(m_timerid);
}

void Scheduler::AddTask(std::shared_ptr<ITask> task, time_t interval)
{
    if(nullptr == task)
    {
        return;
    }
    
    std::unique_lock<std::mutex> lock(m_mutex);
    time_t exec_time;

    task->m_execution_time = std::time(nullptr) + interval;

    if(nullptr == m_curr_task)
    {
        m_curr_task = task;
        exec_time = task->m_execution_time;
    }
    else
    {
        CompareTasks compare;
        if(compare(m_curr_task, task))
        {
            m_tasks.Push(m_curr_task);
            m_curr_task = task;
        }
        else
        {
            m_tasks.Push(task);
        }

        exec_time = m_curr_task->m_execution_time;
    }

    UpdateTimer(exec_time);
}

bool Scheduler::CompareTasks::operator()(const std::shared_ptr<ITask>& task_lhs,
                                const std::shared_ptr<ITask>& task_rhs)
{
    return (0 < ::difftime((*task_lhs).m_execution_time, (*task_rhs).m_execution_time));
}

void Scheduler::OnTimer(::sigval sv)
{
   static_cast<Scheduler*>(sv.sival_ptr)->OnTimerHandler();
}

void Scheduler::OnTimerHandler()
{
    m_mutex.lock();

    std::shared_ptr<ITask> tmp_task = m_curr_task;
    
    if(m_tasks.IsEmpty())
    {
        m_curr_task = nullptr;
    }
    else
    {
        m_tasks.Pop(m_curr_task);
        UpdateTimer(m_curr_task->m_execution_time);
    }

    m_mutex.unlock();
    tmp_task->Execute();
}

void Scheduler::UpdateTimer(::time_t execution_time)
{
    struct itimerspec its;

    its.it_value.tv_sec = execution_time;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    timer_settime(m_timerid, TIMER_ABSTIME, &its, nullptr);
}

} //namespace dev
