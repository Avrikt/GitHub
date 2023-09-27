/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 21.09.2023								*
* Description: Header file for Input Proxy		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_INPUT_PROXY_HPP
#define MY_PROJECT_INPUT_PROXY_HPP

#include <boost/noncopyable.hpp>    //boost::noncopyable

#include "tasks.hpp"

namespace dev
{

class IInputProxy : private boost::noncopyable
{
public:
    virtual ITask *GetTask(int fd) = 0;
    virtual ~IInputProxy() {}
};

} //namespace dev

#endif //MY_PROJECT_INPUT_PROXY_HPP

