/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - UDP Server            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>          /*printf, perror*/
#include <unistd.h>         /*close*/
#include <string.h>         /*strlen*/

#include "tcp_udp.h"

#define PORT        (8228)
#define BUF         (256U)
#define ERROR       (-1)

int UDPServer(int port);

int main(void)
{
    UDPServer(PORT);

    return 0;
}

int UDPServer(int port)
{
    const char *msg = "I am Avri's Server";
    int sockfd = 0;
    char buffer[BUF] = {0};
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int reps = 5;
    ssize_t bytes_recv = 0;

    sockfd = UDPSocketInitSrv(port, NULL);

    for(; reps > 0; --reps)
    {
        bytes_recv = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
        buffer[bytes_recv] = '\0';
        printf("Message received: %s\n", buffer);
        sleep(1);
        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&cliaddr, sizeof(struct sockaddr));
    }

    return 0;
}