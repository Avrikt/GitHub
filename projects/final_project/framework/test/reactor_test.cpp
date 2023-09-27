/************************************************
* Developer: Avri Kehat							*
* Reviewed by:									*
* Date: 07.09.2023								*
* Description: Test file for Reactor			*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <iostream> 
#include <sys/socket.h>
#include <arpa/inet.h> /*inet_addr*/
#include <netinet/in.h> /*INADDR_ANY*/

#include "singleton.hpp"
#include "reactor.hpp"
#include "tcp_udp.hpp"

using namespace dev;

void TCPAccept(int sockfd);
void TestReactor(void);
void TCPPong(int sockfd);
void Stop(int sockfd);

struct timeval timeout{15,0};

int main ()
{
    TestReactor();
    return 0;
}

void TestReactor(void)
{
    Reactor *r = Singleton<Reactor, struct timeval>::GetInstance(timeout);

    int port = 8080;
    char protocol[] = "TCP";
    int tcpfd = Server(protocol, port, NULL);

    int port2 = 1234;
    char protocol2[] = "UDP";
    int tcpfd2 = Server(protocol2, port2, NULL);

    r->AddFd(Reactor::Mode::READ ,tcpfd, TCPAccept);
    r->AddFd(Reactor::Mode::READ ,tcpfd2, Stop);
    r->Run();

    close(tcpfd);
    close(tcpfd2);
}

void TCPAccept(int sockfd)
{
    Reactor *r = Singleton<Reactor, struct timeval>::GetInstance(timeout);
    int newfd = TCPServerAccept(sockfd);
    if (newfd > 0)
    {
        r->AddFd(Reactor::Mode::WRITE, newfd, TCPPong);
    }
}

void TCPPong(int sockfd)
{
    Reactor *r = Singleton<Reactor, struct timeval>::GetInstance(timeout);
    int msg_len = 0;
    char buffer[10] = {0};

    if ((msg_len = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[msg_len] = '\0';
        std::cout<< buffer <<std::endl;

        send(sockfd, "Pong", strlen("Pong"), 0);
    }
    if (0 == msg_len)
    {
        close(sockfd);
        r->RemoveFd(Reactor::Mode::WRITE, sockfd);
    }  
}

void Stop(int sockfd)
{
    (void)sockfd;
    Reactor *r = Singleton<Reactor, struct timeval>::GetInstance(timeout);
    int msg_len = 0;
    char buffer[10] = {0};
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    if ((msg_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len)) > 0)
    {
        buffer[msg_len] = '\0';
        std::cout<< buffer <<std::endl;
    }

    close(sockfd);
    r->Stop();
}
