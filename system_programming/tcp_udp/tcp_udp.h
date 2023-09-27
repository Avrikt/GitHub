/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - TCP/UDP Header        	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/
#ifndef MY_PROJECT_TCP_UDP_H
#define MY_PROJECT_TCP_UDP_H

#include <sys/socket.h>     /*socket, socklen_t*/
#include <netinet/in.h>     /*sockaddr_in*/

int UDPSocketInitSrv(int port_num, const char *ip);

int UDPSocketInitCl(struct sockaddr_in *srv_addr, int port_num, const char *ip);

int UDPInitBrdcst(struct sockaddr_in *srv_addr, int port_num, const char *ip);

int TCPInitSocketSrv(int port_num, const char *ip, int backlog);

int TCPInitSocketCl(int port_num, const char *ip);

int TCPAccept(int fd);

#endif /*MY_PROJECT_TCP_UDP_H*/