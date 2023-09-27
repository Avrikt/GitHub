/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - UDP Client            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>          /*printf, perror*/
#include <unistd.h>         /*close*/
#include <string.h>         /*strlen*/

#include "tcp_udp.h"

#define PORT        (8228)
#define SRV_ADDR    ("10.1.255.255")
#define BUF         (256U)
#define ERROR       (-1)

int main(void)
{
    const char *msg = "Ping";
    struct sockaddr_in serv;
    int reps = 5;
    int sockfd = 0;
    char buffer[BUF] = {0};
    ssize_t bytes_recv = 0;

    sockfd = UDPSocketInitCl(&serv, PORT, NULL);

    for(; reps > 0; --reps)
    {
        sendto(sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr *)&serv, sizeof(struct sockaddr));
        
        bytes_recv = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        buffer[bytes_recv] = '\0';
        printf("%s\n", buffer);
    }

    close(sockfd);
    
    return 0;
}