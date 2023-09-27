/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Harel Salhuv					*
* Date: 27.04.2023							*
* Description: Test file for dhcp			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include "dhcp.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/


/********************************FORWARD DECLERATIONS*******************************************/
static void TestDHCP(void);

static void TestFullDHCP(void);

int main(void)
{
	TestDHCP();
	TestFullDHCP();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
static void TestDHCP(void)
{
	unsigned char subnet[BYTES_IN_IP] = {192, 5, 13, 20};
	unsigned char network[BYTES_IN_IP] = {192, 5, 13, 0};
	unsigned char broadcast[BYTES_IN_IP] = {192, 5, 13, 255};
	unsigned char req2[BYTES_IN_IP] = {182, 5, 13, 10};
	unsigned char req3[BYTES_IN_IP] = {192, 5, 13, 132};
	unsigned char none_valid[BYTES_IN_IP] = {192, 5, 13, 24};
	unsigned char none_valid2[BYTES_IN_IP] = {182, 3, 14, 10};
	unsigned char res_ip[BYTES_IN_IP] = {0};
	dhcp_t *dhcp = DHCPCreate(subnet, 24);

	HEADER(DHCPCreate);
	TEST(NULL != dhcp, DHCPCreate);
	TEST(253 == DHCPCountFree(dhcp), DHCPCountFree);
	
	HEADER(DHCPAllocateIp);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, broadcast, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, req2, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, network, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, network, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, req3, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);
	TEST(SUCCESS == DHCPAllocateIp(dhcp, subnet, res_ip), DHCPAllocateIp);
	printf("Returned IP: %d.%d.%d.%d\n", res_ip[0], res_ip[1], res_ip[2], res_ip[3]);

	HEADER(DHCPCountFree);
	TEST(247 == DHCPCountFree(dhcp), DHCPCountFree);
	DHCPAllocateIp(dhcp, subnet, res_ip);
	DHCPAllocateIp(dhcp, subnet, res_ip);
	DHCPAllocateIp(dhcp, subnet, res_ip);
	DHCPAllocateIp(dhcp, subnet, res_ip);
	TEST(243 == DHCPCountFree(dhcp), DHCPCountFree);


	HEADER(DHCPFreeIp);
	TEST(SUCCESS == DHCPFreeIp(dhcp, req3), DHCPFreeIp);
	TEST(244 == DHCPCountFree(dhcp), DHCPCountFree);
	TEST(DOUBLE_FREE == DHCPFreeIp(dhcp, req3), DHCPFreeIp - Double);
	TEST(INVALID_FREE == DHCPFreeIp(dhcp, network), DHCPFreeIp - Invalid);
	TEST(INVALID_FREE == DHCPFreeIp(dhcp, broadcast), DHCPFreeIp - Invalid);
	TEST(INVALID_FREE == DHCPFreeIp(dhcp, none_valid), DHCPFreeIp - Invalid);
	TEST(INVALID_FREE == DHCPFreeIp(dhcp, none_valid2), DHCPFreeIp - Invalid);

	DHCPDestroy(dhcp);
}

static void TestFullDHCP(void)
{
	unsigned char subnet[BYTES_IN_IP] = {192, 5, 13, 20};
	unsigned char res_ip[BYTES_IN_IP] = {0};
	dhcp_t *dhcp = DHCPCreate(subnet, 24);
	size_t i = 0;

	HEADER(Test Full DHCP);
	for (i = 0; i < 256; ++i)
	{
		DHCPAllocateIp(dhcp, subnet, res_ip);
	}

	TEST(FULL == DHCPAllocateIp(dhcp, subnet, res_ip), DHCPFull);
	
	DHCPDestroy(dhcp);
}