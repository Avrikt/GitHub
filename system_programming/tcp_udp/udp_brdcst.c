/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - UDP Broadcast        	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/
#include "string.h"

#include "tcp_udp.h"

#define PORT (8228)
#define NET_IP ("10.1.255.255")

int main(void)
{
    const char *msg = "Welcome to Avri's broadcast..\nThe show will start momentarily\n";
    int sockfd = 0;
    struct sockaddr_in brdcast;

    sockfd = UDPInitBrdcst(&brdcast, PORT, NET_IP);

    sendto(sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr *)&brdcast, sizeof(struct sockaddr));

    return 0;
}