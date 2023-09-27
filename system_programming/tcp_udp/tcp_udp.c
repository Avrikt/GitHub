/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - TCP/UDP Source        	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>          /*printf, perror*/
#include <string.h>         /*strlen, memset*/
#include <unistd.h>         /*close*/
#include <arpa/inet.h>      /*inet_pton*/

#include "tcp_udp.h"

#define IP_PROTOCOL (0)
#define ERROR       (-1)

static int SocketInit(struct sockaddr_in *srv_addr, int port_num, const char *ip, int type, int optname);

/************************************ UDP Functions *******************************************/
int UDPSocketInitCl(struct sockaddr_in *srv_addr, int port_num, const char *ip)
{
    return SocketInit(srv_addr, port_num, ip, SOCK_DGRAM, SO_REUSEADDR);
}

int UDPSocketInitSrv(int port_num, const char *ip)
{
    struct sockaddr_in srv_add;
    int sockfd = SocketInit(&srv_add, port_num, ip, SOCK_DGRAM, SO_REUSEADDR);

    if(-1 == bind(sockfd, (struct sockaddr *)&srv_add, sizeof(srv_add)))
    {
        perror("bind");
        close(sockfd);
        return ERROR;
    }

    return sockfd;
}

int UDPInitBrdcst(struct sockaddr_in *srv_addr, int port_num, const char *ip)
{
    return SocketInit(srv_addr, port_num, ip, SOCK_DGRAM, SO_BROADCAST);

}

/************************************ TCP Functions *******************************************/
int TCPInitSocketSrv(int port_num, const char *ip, int backlog)
{
    struct sockaddr_in srv_addr;
    int sockfd = SocketInit(&srv_addr, port_num, ip, SOCK_STREAM, SO_REUSEADDR);
    
    if(-1 == bind(sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)))
    {
        perror("bind");
        close(sockfd);
        return ERROR;
    }

    if(-1 == listen(sockfd , backlog))
    {
        perror("listen");
        close(sockfd);
        return ERROR;
    }

    return sockfd;
}

int TCPAccept(int fd)
{
    struct sockaddr_in cl;
    socklen_t len = sizeof(cl);
    int new_fd = accept(fd, (struct sockaddr*)&cl, &len);
    
    if(ERROR == new_fd)
    {
        perror("accept ");
        close(fd);
    }

    return new_fd;
}

int TCPInitSocketCl( int port_num, const char *ip)
{
    struct sockaddr_in srv_addr;
    int sockfd = SocketInit(&srv_addr, port_num, ip, SOCK_STREAM, SO_REUSEADDR);
    
    if(-1 == (connect(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr))))
    {
        perror("accept ");
        close(sockfd);
        return ERROR;
    }

    return sockfd;
}

/************************************ STATIC FUNCTIONS *******************************************/

static int SocketInit(struct sockaddr_in *srv_addr, int port_num, const char *ip, int type, int optname)
{
    int sockfd = 0;
    int brdcst_enable = 1;

    if(-1 == (sockfd = socket(AF_INET, type, IP_PROTOCOL)))
    {
        perror("socket: ");
        return ERROR; 
    }

    if(-1 == setsockopt(sockfd,  SOL_SOCKET, optname, &brdcst_enable, sizeof(brdcst_enable)))
    {
        perror("setsockopt: ");
        return ERROR; 
    }

    memset(&srv_addr->sin_zero, 0, 8);

    srv_addr->sin_family = AF_INET;
    srv_addr->sin_port = htons(port_num);
    if(NULL == ip)
    {
        srv_addr->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        inet_pton(AF_INET, ip, &srv_addr->sin_addr);
    }

    return sockfd;
}