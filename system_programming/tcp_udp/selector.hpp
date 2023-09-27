/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Noam Gazit     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - Selector HPP file    	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#ifndef MY_PROJECT_SELECTOR_HPP
#define MY_PROJECT_SELECTOR_HPP

#include <sys/select.h> //fd_set
#include <map>          //std::map

namespace dev
{

class ServerSelector
{
public:
    ServerSelector(int interval = 0);
    ~ServerSelector();
    int SelecRun();
    void Register(int fd, int (*handler)(int));

protected:
    ServerSelector(const ServerSelector& other);
    ServerSelector& operator=(const ServerSelector& other);

private:
    fd_set m_readfd;
    int m_max_fd;
    const int m_interval;
    std::map<int, int (*)(int)>m_fd_map;
};

int UDPHandler(int fd);

int TCPConnection(int fd);

int TCPHandler(int fd);

int StdinHandler(int fd);

} //namespace dev


#endif //MY_PROJECT_SELECTOR_HPP
