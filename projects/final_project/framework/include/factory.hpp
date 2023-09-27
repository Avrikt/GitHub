/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai   				    *
* Date: 22.08.2023						    	*
* Description: Header file for Factory       	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/
#ifndef MY_PROJECT_FACTORY_HPP
#define MY_PROJECT_FACTORY_HPP

#include <functional>
#include <unordered_map>

namespace dev
{

template<typename BASE, typename KEY, typename... ARGS>
class Factory
{
public:
    Factory() = default;
    ~Factory() = default;

    Factory(const Factory &other) = delete;
    Factory &operator=(const Factory &other) = delete;
    Factory(Factory&& other) = delete;
    Factory& operator=(Factory&& other) = delete;

    BASE* Create(const KEY& key, ARGS... args);  //args are passed to the ctor of obj.
    void Add(const KEY& key, const std::function<BASE*(ARGS...)> func);

private:
    std::unordered_map<KEY, std::function<BASE*(ARGS...)>> m_map;
};

template <typename BASE, typename KEY, typename... ARGS>
BASE* Factory<BASE, KEY, ARGS...>::Create(const KEY& key, ARGS ... args) 
{
    return m_map.at(key)(args...);
}

template <typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::Add(const KEY& key, const std::function<BASE*(ARGS...)> func)
{
    m_map[key] = func;
}

} //namespace dev

#endif // MY_PROJECT_THREAD_POOL_HPP