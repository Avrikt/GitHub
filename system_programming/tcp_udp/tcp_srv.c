/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - TCP Server            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf, perror*/
#include <string.h>     /*strlen*/
#include <unistd.h>     /*fork*/

#include "tcp_udp.h"

#define MY_PORT     (4321)
#define MY_ADDR     ("10.1.0.28")
#define CONNECT_LMT (5)
#define ERROR       (-1)
#define BUFF_SIZE   (512)

void TCPServer(int port);

int main(void)
{
    TCPServer(MY_PORT);

    return 0;
}

void TCPServer(int port)
{
    int sockfd = 0, connect_fd = 0;
    const char *msg = "I am Avri's Server";
    char buffer[BUFF_SIZE] = {0};
    
    sockfd = TCPInitSocketSrv(port, MY_ADDR, CONNECT_LMT);

    while(1)
    {
        connect_fd = TCPAccept(sockfd);

        if(-1 == connect_fd)
        {
            return;
        }

        if(0 == fork())
        {
            send(connect_fd, msg, strlen(msg) + 1, 0);
            
            recv(connect_fd, buffer, sizeof(buffer), 0);

            printf("Message recieved: \x1B[1;33m%s\x1B[0m\n", buffer);
        }
        
        close(connect_fd);
        sleep(1);
    }
    
    shutdown(sockfd, SHUT_RDWR);
}