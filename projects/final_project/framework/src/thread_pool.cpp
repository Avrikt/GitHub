/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Yotam Sasson   				*
* Date: 15.08.2023							*
* Description: Source file for Thread Pool	*
* Version: 2.1                              *
* Status: Approved                          *
*********************************************/

/************************************LIBRARIES**************************************************/

#include "thread_pool.hpp"

namespace dev
{

class ThreadPool::StopException : public std::runtime_error
{
public:
    StopException() : runtime_error("Stop Exception") {}
};

class ThreadPool::StopTask : public ITask
{
    void Execute() {throw StopException();}
};

class ThreadPool::PauseTask : public ITask
{
public:
    PauseTask(ThreadPool &other_) : m_tp(other_) {}
    void Execute();

private:
    ThreadPool &m_tp;
};

void ThreadPool::PauseTask::Execute()
{
    std::unique_lock<std::mutex> lock(m_tp.m_paused_mutex);
    m_tp.m_pause_sem.post();
    if(m_tp.m_is_paused.load(std::memory_order_seq_cst))
    m_tp.m_pause_cond.wait(lock);
}

/************************************ Thread Pool *******************************************/
ThreadPool::ThreadPool(std::size_t num_threads, int niceness, int sched_policy) :
            m_pause_sem(0), m_is_paused(false), m_niceness(niceness), m_sched_policy(sched_policy)
{
    AddThreads(num_threads);
}

ThreadPool::~ThreadPool()
{
    TerminateThreads(m_workers.size());
}

void ThreadPool::Pause() noexcept
{
    m_is_paused.store(true, std::memory_order_release);
    std::shared_ptr<ITask> pause_task = std::make_shared<PauseTask>(*this);
    
    for(size_t i = 0; i < m_workers.size(); ++i)
    {
        AddTask(pause_task, static_cast<Priority>(HIGH + 1));
    }

    for(size_t i = 0; i < m_workers.size(); ++i)
    {
        m_pause_sem.wait();
    }
}

void ThreadPool::Resume() noexcept
{
    m_is_paused.store(false, std::memory_order_release);
    m_pause_cond.notify_all();
}

void ThreadPool::SetNumThreads(std::size_t num_threads)
{
    int diff = num_threads - m_workers.size();
    if (diff > 0)
    {
        AddThreads(diff);
    }
    else
    {
        TerminateThreads(diff * -1);
    }

}

void ThreadPool::AddTask(const std::shared_ptr<ITask> task, Priority priority)
{
    m_tasks.Push(std::make_pair(priority, task));
}

void ThreadPool::WorkerThread()
{
    std::pair<Priority, std::shared_ptr<ITask>> task_pair;
    
    while(true)
    {
        try
        {
            m_tasks.Pop(task_pair);
            task_pair.second->Execute();
        }
        catch(const StopException &e) 
        {
            m_idlers.Push(std::this_thread::get_id());
            break;
        }
        catch(...) {}
    }
}

void ThreadPool::TerminateThreads(std::size_t num_of_threads)
{
    std::shared_ptr<ITask> sh_ptr = std::make_shared<StopTask>();
    for (size_t i = 0; i < num_of_threads; ++i)
    {
        AddTask(sh_ptr, static_cast<Priority>(HIGH + 2));
    }
    
    if(m_is_paused.load())
    {
        for (size_t i = 0; i < num_of_threads; ++i)
        {
            m_pause_cond.notify_one();
        }
    }

    for (size_t i = 0; i < num_of_threads; ++i)
    {
        std::thread::id tid;
        m_idlers.Pop(tid);
        if ((m_workers.find(tid) != m_workers.end()) && m_workers.at(tid).joinable())
        {
            m_workers.at(tid).join();
            m_workers.erase(tid);
        }
    }
}

void ThreadPool::AddThreads(std::size_t num_of_threads)
{
    if(m_is_paused.load())
    {
        std::shared_ptr<ITask> pause_task = std::make_shared<PauseTask>(*this);
    
        for(size_t i = 0; i < num_of_threads; ++i)
        {
            AddTask(pause_task, static_cast<Priority>(HIGH + 1));
        }
    }
    
    try
    {
        struct sched_param param;
        param.sched_priority = m_niceness;
        for (unsigned int i = 0; i < num_of_threads; ++i)
        {
            std::thread thread([this] { WorkerThread();});

            if (0 != pthread_setschedparam(thread.native_handle(), m_sched_policy, &param))
            {
                throw std::runtime_error("pthread_setschedparam failed");
            }
            m_workers.emplace(thread.get_id(), std::move(thread));
        }
    }
    catch (const std::exception &e)
    {
        TerminateThreads(m_workers.size());
        throw e;
    }

    if(m_is_paused.load())
    {
        for(size_t i = 0; i < num_of_threads; ++i)
        {
            m_pause_sem.wait();
        }
    }
    
}

}