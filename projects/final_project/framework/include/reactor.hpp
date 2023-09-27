/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 07.09.2023								*
* Description: Header file for Reactor		*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#ifndef MY_PROJECT_REACTOR_HPP
#define MY_PROJECT_REACTOR_HPP

#include <sys/select.h>         //fd_set
#include <boost/function.hpp>   //std::function
#include <boost/noncopyable.hpp> // boost::noncopyable
#include <map>                  //std::map

namespace dev
{

class Reactor : private boost::noncopyable
{
public:
    enum Mode { READ, WRITE, EXCEPT };
    typedef std::function<void(int)> ActionFunc;

    explicit Reactor(const timeval &timeout);
    // ~Reactor() = default;

    void AddFd(Mode mode, int fd, const ActionFunc &proc);
    void RemoveFd(Mode mode, int fd);

    void Run();
    void Stop();

private:
    typedef std::pair<Mode, int> ModeAndFD;

    timeval m_timeout;
    fd_set m_readfds;
    fd_set m_writefds;
    fd_set m_exceptfds;
    int m_maxfd;
    bool m_runFlag;
    std::map<ModeAndFD, ActionFunc> m_fdToFuncs;
    std::map<Mode, fd_set*> m_fdSetMap;
};

} //namespace dev

#endif //MY_PROJECT_REACTOR_HPP