/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 21.09.2023								*
* Description: Header file for Input Mediator	*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_INPUT_MEDIATOR_HPP
#define MY_PROJECT_INPUT_MEDIATOR_HPP

#include <unordered_map>

#include "reactor.hpp"
#include "thread_pool.hpp"
#include "input_proxy.hpp"

namespace dev
{

class InputMediator : private boost::noncopyable
{
public:
    InputMediator(Reactor &r_, ThreadPool &tp_);

    void Add(Reactor::Mode mode, int fd, IInputProxy &input);

private:
    std::unordered_map<int, IInputProxy&> m_proxyMap;
    Reactor &m_reactor;
    ThreadPool &m_threadPool;
};

} //namespace dev

#endif //MY_PROJECT_INPUT_MEDIATOR_HPP