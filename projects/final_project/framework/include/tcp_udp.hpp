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

typedef enum status
{
	SUCCESS = 0,
	FAIL = -1,
	STDIN_CLOSE = -2,
    CLIENT_CLOSE = -3
}status_t;

/*
    return the server fd
*/
int TCPInitServer(int port, char *address, int backlog, int optname);
/*
    return the client fd
*/
int TCPClient(int port , char *address, int optname);
/*
    return the new socket fd after accepting the connection 
*/
int TCPServerAccept(int serverfd);
/*
    return the socket fd
*/
int SocketInitNonBlocking(int port, char *address, struct sockaddr_in *server_addr, int type, int optname);

int SocketInitBlocking(int port, char *address, struct sockaddr_in *server_addr, int type, int optname);
/*
    return the socket fd to an address 
*/
int SocketInitAndBind(int port, char *address, int type, int optname);


int Server(char *protocol, int port, char *address);


#endif /*MY_PROJECT_TCP_UDP_H*/