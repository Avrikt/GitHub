/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 21.09.2023								*
* Description: Header file for NBD Proxy		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_NBD_PROXY_HPP
#define MY_PROJECT_NBD_PROXY_HPP

#include <iostream>

#include "input_proxy.hpp"

namespace dev
{

class StubTask : public ITask 
{
public:
    virtual void Execute();
};

void StubTask::Execute()
{
    std::cout << "Stub Task Executed\n";
}

class NBDProxy : public IInputProxy
{
public:
    virtual ITask *GetTask(int fd);
};

ITask *NBDProxy::GetTask(int fd)
{
    (void)fd;
    return new StubTask;
}

} //namespace dev

#endif //MY_PROJECT_NBD_PROXY_HPP