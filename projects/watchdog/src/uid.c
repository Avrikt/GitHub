/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 01.03.23							*
* Description: Source file for UID  		*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*perror*/
#include <string.h> /*memcmp*/
#include <unistd.h> /*getpid*/
#include <ifaddrs.h> /*getifaddrs, struct ifaddrs*/
#include <arpa/inet.h> /*inet_ntop, sockaddr_in*/

#include "uid.h"

/***************************PROTOTYPES AND GLOBAL VARIABLES**************************************/

static int GetIP(unsigned char *ip);

static size_t uid_count = 0;

/************************************Functions***************************************************/

m_uid_t UIDCreate(void)
{
    m_uid_t new_uid = {0};

    if (GetIP(new_uid.ip))
    {
        return UIDGetBad();
    };

    new_uid.time_stamp = time(NULL);
    new_uid.counter = ++uid_count;
    new_uid.pid = getpid();

    return new_uid;
}

int UIDIsSame(m_uid_t lhs, m_uid_t rhs)
{
    return ((0 == difftime(lhs.time_stamp, rhs.time_stamp)) &&
            (lhs.counter == rhs.counter) &&
            (lhs.pid == rhs.pid) &&
            0 == memcmp(lhs.ip, rhs.ip, sizeof(lhs.ip)));
}

m_uid_t UIDGetBad(void)
{
    m_uid_t bad_uid = {0};

    return bad_uid;
}


static int GetIP(unsigned char *ip)
{
    struct ifaddrs *ifaddr = NULL, *ifa = NULL;

    if (-1 == getifaddrs(&ifaddr))
    {
        perror("getifaddrs");
        return 1;
    }

    for (ifa = ifaddr; NULL != ifa; ifa = ifa->ifa_next)
    {
        if (NULL != ifa->ifa_addr && AF_INET == ifa->ifa_addr->sa_family)
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr), (char *)ip, LINUX_IP_SIZE);
        }
    }
    freeifaddrs(ifaddr);

    return 0;
}
