/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Noam Gazit     				    *
* Date: 25.07.2023						    	*
* Description: TCP/UDP - Selector test file    	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#include <unistd.h> //STDIN_FILENO

#include "selector.hpp"
#include "tcp_udp.h"

using namespace dev;

const int PORT(8228);
const int BRD_PORT(2882);
const int BACKLOG = 5;
const char *IP = "10.1.0.28";

int main(void)
{
    ServerSelector selector(7);
    int tcp_fd = TCPInitSocketSrv(PORT, IP, BACKLOG);
    int udp_fd = UDPSocketInitSrv(PORT, NULL);
    int std_fd = STDIN_FILENO;
    
    selector.Register(tcp_fd, TCPConnection);
    selector.Register(udp_fd, UDPHandler);
    selector.Register(std_fd, StdinHandler);
    selector.SelecRun();

    return 0;
}