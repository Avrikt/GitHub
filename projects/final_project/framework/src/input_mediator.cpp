/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 21.09.2023								*
* Description: Source file for Input Mediator	*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#include "input_mediator.hpp"

namespace dev
{

InputMediator::InputMediator(Reactor &r_, ThreadPool &tp_) : m_reactor(r_), m_threadPool(tp_) {}

void InputMediator::Add(Reactor::Mode mode, int fd, IInputProxy &input)
{
    m_proxyMap.insert({fd, input});

    m_reactor.AddFd(mode, fd, [this](int fd)
    {
        ITask *task = m_proxyMap.at(fd).GetTask(fd);

        if(task)
        {
            m_threadPool.AddTask(std::shared_ptr<ITask>(task));
        }
    });
}

} //namespace dev