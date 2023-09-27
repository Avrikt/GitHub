/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 07.09.2023								*
* Description: Source file for Reactor			*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <iostream>

#include "reactor.hpp"

/************************************Functions***************************************************/

namespace dev
{

enum ErrorCodes{ ERROR = -1};

Reactor::Reactor(const timeval &timeout): m_timeout(timeout), m_runFlag(true),
                                          m_fdSetMap{{READ,     &m_readfds},
                                                     {WRITE,    &m_writefds},
                                                     {EXCEPT,   &m_exceptfds}}
{
    FD_ZERO(&m_readfds);
    FD_ZERO(&m_writefds);
    FD_ZERO(&m_exceptfds);
}

void Reactor::AddFd(Mode mode, int fd, const ActionFunc &proc)
{
    FD_SET(fd, m_fdSetMap[mode]);
    m_fdToFuncs[{mode, fd}] = proc;
    m_maxfd = std::max(fd + 1, m_maxfd);
}

void Reactor::RemoveFd(Mode mode, int fd)
{
    auto it = m_fdToFuncs.find({mode, fd});
    if(it != m_fdToFuncs.end())
    {
        m_fdToFuncs.erase(it);
        FD_CLR(fd, m_fdSetMap[mode]);
    }
}

void Reactor::Run()
{
    while(m_runFlag && !m_fdToFuncs.empty())
    {
        fd_set tmp_read   = m_readfds;
        fd_set tmp_write  = m_writefds;
        fd_set tmp_except = m_exceptfds;

        struct timeval tmp_timeout = m_timeout;

        int fd_ready;
        if (ERROR == (fd_ready = select(m_maxfd, &tmp_read, &tmp_write, &tmp_except, &tmp_timeout)))
        {
            throw std::runtime_error("select() failed");
        }
        else if(0 == fd_ready)
        {
            std::cout << "Timeout expired\n";
        }
        else if(0 < fd_ready)
        {
            std::map<ModeAndFD, ActionFunc> tmp_fdFuncs(m_fdToFuncs);

            for(auto it : tmp_fdFuncs)
            {
                if(!m_runFlag)
                {
                    break;
                }
                
                int current_fd = it.first.second;
                if( FD_ISSET(current_fd, &tmp_read) || 
                    FD_ISSET(current_fd, &tmp_write) || 
                    FD_ISSET(current_fd, &tmp_except))
                {
                        ModeAndFD key = {it.first.first, current_fd};
                        if (m_fdToFuncs.find(key) != tmp_fdFuncs.end())
                        {
                            try
                            {
                                it.second(current_fd);
                            }
                            catch(...) {}
                            
                        }
                }
            }
        }
    }
}

void Reactor::Stop()
{
    m_runFlag = false;
}

	
} //namespace dev
