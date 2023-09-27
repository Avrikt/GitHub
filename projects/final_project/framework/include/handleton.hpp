/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Makrov						*
* Date: 23.08.2023							    *
* Description: Header file for Handleton		*
* Version: 1.1                              	*
* Status: Approved                            	*
*************************************************/

#ifndef MY_PROJECT_HANDLETON_HPP
#define MY_PROJECT_HANDLETON_HPP
#ifdef I_AM_SHARED_LIBRARY

#include <mutex>
#include <atomic>

namespace dev
{

template<typename OBJECT, typename... ARGS>
class Handleton 
{
public:
    static OBJECT* GetInstance(ARGS... args);
    
    Handleton(const Handleton &other) = delete;
    Handleton &operator=(const Handleton &other) = delete;
    Handleton(Handleton&& other) = delete;
    Handleton& operator=(Handleton&& other) = delete;

private:
    using Lock = std::unique_lock<std::mutex>;
    ~Handleton() = delete; 
    Handleton() = delete;

    static void CleanUp();

    static OBJECT* s_instance;
    static std::mutex s_mtx;
};

template<typename OBJECT, typename... ARGS>
OBJECT *Handleton<OBJECT, ARGS...>::s_instance = nullptr; 

template<typename OBJECT, typename... ARGS>
std::mutex Handleton<OBJECT, ARGS...>::s_mtx;

template<typename OBJECT, typename... ARGS>
OBJECT* Handleton<OBJECT, ARGS...>::GetInstance(ARGS... args)
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
void Handleton<OBJECT, ARGS...>::CleanUp()
{
    delete s_instance;
    s_instance = reinterpret_cast<OBJECT*>(0xDEADBEEF);
}

} //namespace dev

#else
namespace dev
{
template<typename OBJECT, typename... ARGS>
class Handleton
{
public:
    static OBJECT* GetInstance(ARGS... args);
private:
    Handleton();
    ~Handleton();
};

} //namespace dev
#endif // I_AM_SHARED_LIBRARY

#endif //MY_PROJECT_HANDLETON_HPP

