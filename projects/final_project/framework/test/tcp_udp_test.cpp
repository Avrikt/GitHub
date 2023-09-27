#include <iostream>
#include <sys/socket.h> /*socket*/
#include <netinet/in.h> /*INADDR_ANY*/
#include <cstring>
#include <unistd.h> /*close*/

#include "tcp_udp.hpp"

#define PORT_TCP (8080)
#define IP (NULL)

#define PORT_UDP (1234)
#define BROADCAST_IP (NULL)

void TestTCPClient(void);
void TestUDPClient(void);

int main (int argc, char* argv[])
{
    (void)argc;

    std::string str = argv[1];

    if (str == "tcp")
    {
        TestTCPClient();
    }
    else if (str == "udp")
    {
        TestUDPClient();
    }

    return 0;
}

void TestTCPClient(void)
{
    int pingfd1 = 0;
    pingfd1 = TCPClient(PORT_TCP, IP, SO_REUSEADDR);

    for (int i = 0; i < 5; ++i)
    {
        send(pingfd1, "Ping", strlen("Ping"), 0);

        char buffer[225] = {0};
        int msg_len = recv(pingfd1, buffer, sizeof(buffer), 0);
        if (msg_len < 0)
        {
            perror("recv");
            close(pingfd1);
            return;
        }

        buffer[msg_len] = '\0';
       std::cout <<buffer<< std::endl;

        sleep(1);
    }

    close(pingfd1);
}

void TestUDPClient(void)
{
    int sockfd;
    struct sockaddr_in client_addr;
    
    sockfd = SocketInitBlocking(PORT_UDP, BROADCAST_IP, &client_addr, SOCK_DGRAM, SO_BROADCAST);

    if (sendto(sockfd, "Exit", strlen("Exit"), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("sendto");
        close(sockfd);
        return;
    }

    close(sockfd);
}
