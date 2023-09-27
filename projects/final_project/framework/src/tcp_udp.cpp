/************************************************
* Developer: Avri Kehat						    *
* Reviewed by:									*
* Date: 07.09.2023								*
* Description: Source file for TCP_UDP			*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

#include <stdio.h> /*print*/
#include <sys/socket.h> /*socket*/
#include <netinet/in.h> /*INADDR_ANY*/
#include <string.h> /*strlen , strcmp*/
#include <unistd.h> /*close*/
#include <arpa/inet.h> /*inet_addr*/
#include <fcntl.h> /*O_NONBLOCK*/

#include "tcp_udp.hpp"

int Server(char *protocol, int port, char *address)
{
    int sockfd = 0;

    int backlog = 1024;

    if (strcmp(protocol, "TCP") == 0)
    {
        sockfd= TCPInitServer(port, address, backlog, SO_REUSEADDR);
    }
    else if (strcmp(protocol, "UDP") == 0)
    {
       sockfd = SocketInitAndBind(port, address, SOCK_DGRAM, SO_REUSEADDR);
    }

    return sockfd;
}


int SocketInitNonBlocking(int port , char *address, struct sockaddr_in *server_addr, int type, int optname)
{
    int sockfd;
    
    if ((sockfd = socket(AF_INET, type, 0)) < 0)
    {
        perror("Socket creation failed");
        return FAIL;
    }

    int flag = O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flag) < 0)
    {
        perror("fcntl");
        close(sockfd);
        return FAIL;
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, optname, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return FAIL;
    }

    if (NULL == address)
    {
        server_addr->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        inet_pton(AF_INET, address, &server_addr->sin_addr);
    }
    server_addr->sin_family      = AF_INET;
    server_addr->sin_port        = htons(port);
    memset(server_addr->sin_zero, '\0', 8);

    return sockfd;
}

int SocketInitBlocking(int port , char *address, struct sockaddr_in *server_addr, int type, int optname)
{
    int sockfd;
    
    if ((sockfd = socket(AF_INET, type, 0)) < 0)
    {
        perror("Socket creation failed");
        return FAIL;
    }

    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, optname, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return FAIL;
    }

    if (NULL == address)
    {
        server_addr->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        inet_pton(AF_INET, address, &server_addr->sin_addr);
    }
    server_addr->sin_family      = AF_INET;
    server_addr->sin_port        = htons(port);
    memset(server_addr->sin_zero, '\0', 8);

    return sockfd;
}

int SocketInitAndBind(int port, char *address, int type, int optname)
{
    int sockfd;
    struct sockaddr_in sock_addr;
    
    sockfd = SocketInitNonBlocking(port, address, &sock_addr, type, optname);

    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        return FAIL;
    }

    return sockfd;
}

int TCPClient(int port, char *address, int optname)
{
    int clientfd;
    struct sockaddr_in server_addr;
    
    clientfd = SocketInitBlocking(port, address, &server_addr, SOCK_STREAM, optname);

    if (connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect:");
        close(clientfd);
        return FAIL;
    }

    return clientfd;
}

int TCPInitServer(int port, char *address, int backlog, int optname)
{
    int serverfd = 0;
    
    serverfd = SocketInitAndBind(port, address, SOCK_STREAM, optname);

    if (listen(serverfd, backlog) < 0)
    {
        perror("listen");
        close(serverfd);
        return FAIL;
    }

    return serverfd;
}

int TCPServerAccept(int serverfd)
{
    int newfd = 0;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    newfd = accept(serverfd, (struct sockaddr *)&client_addr, &client_len);
    if (newfd < 0)
    {
        perror("accept");
        close(serverfd);
        return FAIL;
    }

    return newfd;
}
