/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Dave Hairapetian					*
* Date: 13.09.2023								*
* Description: Header file for Scheduler		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_SCHEDULER_HPP
#define MY_PROJECT_SCHEDULER_HPP

#include <signal.h>

#include "singleton.hpp"
#include "pq.hpp"
#include "waitable_q.hpp"

namespace dev
{

class Scheduler
{
public:
    class ITask;

    void AddTask(std::shared_ptr<ITask> task, time_t interval);

    friend class Singleton<Scheduler>;

    class ITask
    {
    public:
        inline virtual ~ITask() {}
        virtual void Execute() = 0;
        friend class Scheduler;
    private:
        ::time_t m_execution_time;
    };

private:
    Scheduler();
    ~Scheduler();

    class CompareTasks
    {
    public:
        inline bool operator()(const std::shared_ptr<ITask>& task_lhs,
                                const std::shared_ptr<ITask>& task_rhs);
    };

    static void OnTimer(::sigval sv);
    void OnTimerHandler();
    void UpdateTimer(::time_t execution_time);

    ::timer_t m_timerid;
    std::mutex m_mutex;
    
    std::shared_ptr<ITask> m_curr_task;

    WaitableQueue<PQ<
        std::shared_ptr<ITask>,
        std::vector<std::shared_ptr<ITask>>,
        typename Scheduler::CompareTasks>> m_tasks;
};

} //namespace dev

#endif //MY_PROJECT_SCHEDULER_HPP

