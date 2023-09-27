/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Harel Salhuv					*
* Date: 27.04.2023							*
* Description: Source file for dhcp			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc, free*/
#include <limits.h> /*CHAR_BIT*/
#include <string.h> /*memcpy*/

#include "dhcp.h"
#include "trie.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/
#define NETWORK     (0x00000000)
#define SERVER      (0xFFFFFFFE)
#define BROADCAST   (0xFFFFFFFF)
#define BITS_IN_IP  (BYTES_IN_IP * CHAR_BIT)
#define BITS_TO_ALLOC(dhcp) (BITS_IN_IP - dhcp->bits_in_subnet)

struct dhcp_s
{
    trie_t *trie;
    unsigned char subnet_ip[BYTES_IN_IP];
    size_t bits_in_subnet;
};

static dhcp_status_t InitDHCP(dhcp_t *dhcp);

static void IpToInt(unsigned char *str_ip, unsigned int *int_ip);

static void IntToIP(unsigned char *str_ip, unsigned int *int_ip);

static unsigned int MirrorInt(unsigned int ip);

/************************************Functions***************************************************/
dhcp_t *DHCPCreate(unsigned char *subnet_ip, size_t bits_in_subnet)
{
    dhcp_t *new_dhcp = NULL;
    unsigned int flipped = 0;

    assert(NULL != subnet_ip);
    assert(3 < bits_in_subnet);
    assert(32 > bits_in_subnet);

    new_dhcp = (dhcp_t *) malloc(sizeof(dhcp_t));
    if(NULL == new_dhcp)
    {
        return NULL;
    }

    new_dhcp->trie = TrieCreate();
    if(NULL == new_dhcp->trie)
    {
        free(new_dhcp);
        return NULL;
    }

    IpToInt(subnet_ip, &flipped);
    flipped >>= BITS_IN_IP - bits_in_subnet;
    flipped <<= BITS_IN_IP - bits_in_subnet;
    IntToIP(subnet_ip, &flipped);

    new_dhcp->bits_in_subnet = bits_in_subnet;
    memcpy(new_dhcp->subnet_ip, subnet_ip, BYTES_IN_IP);

    if(SUCCESS != InitDHCP(new_dhcp))
    {
        TrieDestroy(new_dhcp->trie);
        free(new_dhcp);
        return NULL;
    }

    return new_dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp->trie);

    TrieDestroy(dhcp->trie);
    free(dhcp);
}

dhcp_status_t DHCPAllocateIp(dhcp_t *dhcp, unsigned char *requested_ip, unsigned char *result_ip)
{
    unsigned int int_ip = 0;
    unsigned int mask = 0;
    unsigned int subnet_mask = 0;
    dhcp_status_t status = SUCCESS;

    assert(NULL != requested_ip);
    assert(NULL != result_ip);

    IpToInt(requested_ip, &int_ip);
    IpToInt(dhcp->subnet_ip, &subnet_mask);

    status = (dhcp_status_t) TrieInsert(dhcp->trie, int_ip, BITS_TO_ALLOC(dhcp), &int_ip);
    
    mask = (~0 << BITS_TO_ALLOC(dhcp));
    int_ip = ((mask | int_ip) - mask) | subnet_mask; 
    IntToIP(result_ip, &int_ip);
    
    return status;
}

dhcp_status_t DHCPFreeIp(dhcp_t *dhcp, unsigned char *ip_to_free)
{
    unsigned int ip = 0;
    unsigned int host_id = 0;
    unsigned int net_id = 0;

    assert(NULL != dhcp);
    assert(NULL != ip_to_free);

    IpToInt(ip_to_free, &ip);
    IpToInt(dhcp->subnet_ip, &net_id);

    host_id = ip;
    host_id <<= dhcp->bits_in_subnet;
    host_id >>= dhcp->bits_in_subnet;
    
    if (NETWORK >>dhcp->bits_in_subnet == host_id ||
        SERVER >> dhcp->bits_in_subnet == host_id ||
        BROADCAST >> dhcp->bits_in_subnet == host_id ||
        ip >> BITS_TO_ALLOC(dhcp) != net_id >> BITS_TO_ALLOC(dhcp))
    {
        return INVALID_FREE;
    }

    return (dhcp_status_t)TrieRemove(dhcp->trie, ip, BITS_TO_ALLOC(dhcp));
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    
    return (2 << (BITS_TO_ALLOC(dhcp) - 1)) - TrieCount(dhcp->trie, BITS_TO_ALLOC(dhcp));
}

static dhcp_status_t  InitDHCP(dhcp_t *dhcp)
{
    unsigned int address = 0;
    dhcp_status_t status = 0;

    status += TrieInsert(dhcp->trie, NETWORK, BITS_TO_ALLOC(dhcp), &address);
    status += TrieInsert(dhcp->trie, SERVER, BITS_TO_ALLOC(dhcp), &address);
    status += TrieInsert(dhcp->trie, BROADCAST, BITS_TO_ALLOC(dhcp), &address);

    return status;
}

static void IpToInt(unsigned char *str_ip, unsigned int *int_ip)
{
    memcpy(int_ip, str_ip, BYTES_IN_IP);
    *int_ip = MirrorInt(*int_ip);
}

static void IntToIP(unsigned char *str_ip, unsigned int *int_ip)
{
    *int_ip = MirrorInt(*int_ip);
    memcpy(str_ip, int_ip, BYTES_IN_IP);
}

static unsigned int MirrorInt(unsigned int ip)
{
    unsigned int byte1 = ((ip & 0xFF000000) >> 24);
    unsigned int byte2 = ((ip & 0x00FF0000) >> 8);
    unsigned int byte3 = ((ip & 0x0000FF00) << 8);
    unsigned int byte4 = ((ip & 0x000000FF) << 24);
    
    ip = byte1 | byte2 | byte3 | byte4;

    return ip;
}