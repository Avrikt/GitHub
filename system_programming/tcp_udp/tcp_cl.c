/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - TCP Client            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf*/
#include <string.h>     /*memset*/
#include <unistd.h>     /*close*/

#include "tcp_udp.h"

#define DEST_PORT   (8228)
#define DEST_ADDR   ("10.1.0.28")
#define ERROR       (-1)
#define BUFF_SIZE   (512)

int main(void)
{
    int sockfd = 0;
    const char *msg = "Ping";
    char buffer[BUFF_SIZE] = {0};
    int reps = 5;
    
    sockfd = TCPInitSocketCl(DEST_PORT, DEST_ADDR);

    for(;reps > 0; --reps)
    {
        send(sockfd, msg, strlen(msg) + 1, 0);
        
        recv(sockfd, buffer, sizeof(buffer), 0);

        printf("%s\n", buffer);
    }
    return 0;
}