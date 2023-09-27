/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Makrov						*
* Date: 23.08.2023							    *
* Description: Header file for singleton		*
* Version: 1.1                              	*
* Status: Approved                            	*
*************************************************/

#ifndef MY_PROJECT_SINGLETON_HPP
#define MY_PROJECT_SINGLETON_HPP

#include <mutex>
#include <atomic>

namespace dev
{

template<typename OBJECT, typename... ARGS>
class Singleton 
{
public:
    static OBJECT* GetInstance(ARGS... args);
    
    Singleton(const Singleton &other) = delete;
    Singleton &operator=(const Singleton &other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;

private:
    using Lock = std::unique_lock<std::mutex>;
    ~Singleton() = delete; 
    Singleton() = delete;

    static void CleanUp();

    static OBJECT* s_instance;
    static std::mutex s_mtx;
};

template<typename OBJECT, typename... ARGS>
OBJECT *Singleton<OBJECT, ARGS...>::s_instance = nullptr; 

template<typename OBJECT, typename... ARGS>
std::mutex Singleton<OBJECT, ARGS...>::s_mtx;

template<typename OBJECT, typename... ARGS>
OBJECT* Singleton<OBJECT, ARGS...>::GetInstance(ARGS... args)
{
    OBJECT *tmp = s_instance;

    std::atomic_thread_fence(std::memory_order_acquire);
    if (nullptr == s_instance)
    {
        Lock lock(s_mtx);
        if(nullptr == s_instance)
        {
            if(0 == atexit(CleanUp))
            {
                tmp = new OBJECT(args...);
                std::atomic_thread_fence(std::memory_order_release);
                s_instance = tmp;
            }
        }
    }

    return s_instance;
}

template<typename OBJECT, typename... ARGS>
void Singleton<OBJECT, ARGS...>::CleanUp()
{
    delete s_instance;
    s_instance = reinterpret_cast<OBJECT*>(0xDEADBEEF);
}

} //namespace dev

#endif //MY_PROJECT_SINGLETON_HPP

