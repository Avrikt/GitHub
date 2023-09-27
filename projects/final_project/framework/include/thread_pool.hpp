/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Yotam Sasson   				*
* Date: 15.08.2023							*
* Description: Header file for Thread Pool	*
* Version: 2.1                              *
* Status: Approved                          *
*********************************************/

#ifndef MY_PROJECT_THREAD_POOL_HPP
#define MY_PROJECT_THREAD_POOL_HPP

#include <unordered_map>    //unordered_map
#include <queue>            //queue
#include <thread>           //thread::id, thread::hardware_concurrency
#include <mutex>            //mutex
#include <sched.h>          //sched policy

#include "tasks.hpp"
#include "waitable_q.hpp"
#include "pq.hpp"

namespace dev
{


/************************************ Thread Pool *******************************************/
class ThreadPool
{
public:
    ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency(), int niceness = 0, int sched_policy = SCHED_IDLE); // return from hardware_concurrency can be 0
    ~ThreadPool() noexcept;

    ThreadPool(const ThreadPool &other) = delete;
    ThreadPool &operator=(const ThreadPool &other) = delete;

    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;
    
    enum Priority //HIGH must be max value
    {
        LOW,
        MEDIUM,
        HIGH
    };

    void Pause() noexcept; //blocking until all threads finished finished executing current task
    void Resume() noexcept;
    void SetNumThreads(std::size_t num_threads);
    void AddTask(const std::shared_ptr<ITask> task, Priority priority = MEDIUM);

private:
    //Private methods
    void WorkerThread();
    void AddThreads(std::size_t num_of_threads);
    void TerminateThreads(std::size_t num_of_threads);

    //Class members
    class PauseTask;
    class StopException;
    class StopTask;
    
    //Containers
    std::unordered_map<std::thread::id, std::thread> m_workers;
    WaitableQueue<PQ<std::pair<Priority ,std::shared_ptr<ITask> >>> m_tasks;
    WaitableQueue<std::queue<std::thread::id>> m_idlers;
    
    //Synchronization
    std::mutex m_paused_mutex;
    std::condition_variable m_pause_cond;
    boost::interprocess::interprocess_semaphore m_pause_sem;

    std::atomic<bool> m_is_paused;
    const int m_niceness;
    const int m_sched_policy;
};

} //namespace dev

#endif //MY_PROJECT_THREAD_POOL_HPP