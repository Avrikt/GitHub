/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Noam Gazit     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - Selector CPP file    	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/
#include <arpa/inet.h>  //
#include <fstream>      //ofstream
#include <iostream>     //cout ,cerr 
#include <unistd.h>     //close, sleep, read, write
#include <string>       //std::string
#include <cstring>      //strlen, strcmp

#include "selector.hpp"

namespace dev
{

enum ErrorCodes 
{
    ERROR = -1,
    EXIT_FLAG = -2
};

static const std::string currentDateTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X: ", &tstruct);

    return buf;
}

ServerSelector::ServerSelector(int interval): m_max_fd(0), m_interval(interval)
{
    FD_ZERO(&m_readfd);
}

ServerSelector::~ServerSelector()
{
    FD_ZERO(&m_readfd);
    std::map<int, int(*)(int)>::const_iterator it;
    for (it = m_fd_map.begin(); it != m_fd_map.end(); ++it)
    {
        close(it->first);
    }

}

void ServerSelector::Register(int fd, int (*handler)(int))
{
    m_fd_map[fd] = handler;
    FD_SET(fd, &m_readfd);
    m_max_fd = std::max(fd, m_max_fd);
}

int ServerSelector::SelecRun()
{
    bool exit = true;

    while(exit == true)
    {
        struct timeval interval = {m_interval, 0};
        fd_set tmp_fd = m_readfd;
        int wait_flag = 0;

        if(ERROR == (wait_flag = select(m_max_fd + 1, &tmp_fd, NULL, NULL, &interval)))
        {
            std::cerr << "select()";
            return ERROR;
        }

        if(0 == wait_flag)
        {
            std::ofstream logFile("logfile.txt", std::ios_base::app);
            logFile << currentDateTime() << "Time out" << std::endl;
        }
        else
        {
            for (std::map<int, int(*)(int)>::const_iterator it = m_fd_map.begin();
                    it != m_fd_map.end(); ++it)
            {
                if(true == FD_ISSET(it->first, &tmp_fd))
                {
                    int re_val = it->second(it->first);
                    
                    if(ERROR == re_val)
                    {
                        FD_CLR(it->first,&m_readfd);
                        m_fd_map.erase(re_val);
                        std::ofstream logFile("logfile.txt", std::ios_base::app);
                        logFile << currentDateTime() << "TCP connection closed" << std::endl;
                    }

                    if(EXIT_FLAG == re_val)
                    {
                        exit = false;
                        break;
                    }

                    if(re_val != it->first && re_val > 0)
                    {
                        Register(re_val, TCPHandler);
                    }
                }
            }
        }
    }
    
    return 0;
}

int UDPHandler(int fd)
{
    std::string msg = "Pong";
    char buffer[1024] = {'\0'};
    struct sockaddr_in srv;
    socklen_t len = sizeof(struct sockaddr);

    recvfrom(fd, buffer, sizeof(buffer), 0 , (struct sockaddr *)&srv, &len);
    
    std::cout << buffer << std::endl;

    if(0 == strcmp(buffer, "Ping"))
    {
        std::ofstream logFile("logfile.txt", std::ios_base::app);
        logFile << currentDateTime() << "UDP message: " << buffer << std::endl;
        sendto(fd, msg.data(), msg.length() + 1, 0, (struct sockaddr *)&srv, len);
    }

    return fd;
}

int TCPConnection(int fd)
{
    struct sockaddr_in cl;
    socklen_t len = sizeof(cl);
    int new_fd = accept(fd, (struct sockaddr*)&cl, &len);
    
    if(ERROR == new_fd)
    {
        std::cerr << "accept()";
    }

    std::ofstream logFile("logfile.txt", std::ios_base::app);
    logFile << currentDateTime() << "Connection Accepted" << std::endl;

    return new_fd;
}


int TCPHandler(int fd)
{
    std::string msg = "Pong";
    char buffer[512] = {0};
    ssize_t bytes_recv = 0;

    if(0 >= (bytes_recv = recv(fd, buffer, sizeof(buffer), 0)))
    {
        close(fd);
        return ERROR;
    }
    
    std::cout << buffer << std::endl;

    if(0 == strcmp(buffer, "Ping"))
    {
        std::ofstream logFile("logfile.txt", std::ios_base::app);
        logFile << currentDateTime() << "TCP message: " <<  buffer << std::endl;

        send(fd, msg.data(), msg.length() + 1, 0);
    }

    return fd;
}

int StdinHandler(int fd)
{
    char buffer[16] = {0};
    ssize_t bytes_read = 0;

    if(0 < (bytes_read = read(fd, buffer, sizeof(buffer))))
    {
        buffer[bytes_read] = '\0';
        if(0 == strcmp("quit\n", buffer))
        {
            std::ofstream logFile("logfile.txt", std::ios_base::app);
            logFile << currentDateTime() << "Quit" << std::endl;
            close(fd);
            return EXIT_FLAG;
        }
        if(0 == strcmp("ping\n", buffer))
        {
            std::ofstream logFile("logfile.txt", std::ios_base::app);
            logFile << currentDateTime() << "STDIN message: " << buffer;
            bytes_read = write(fd, "pong\n", strlen("pong\n"));
        }
    }

    return fd;
}

} //namespace dev