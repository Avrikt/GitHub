/************************************************
* Developer: Avri Kehat							*
* Reviewed by:									*
* Date: 14.09.2023								*
* Description: Test file for Async Injection	*
* Version: 1.0                              	*
* Status: Sent                              	*
*************************************************/

/************************************LIBRARIES**************************************************/

#include "async_inject.hpp"
#include "utilities.hpp"

using namespace dev;

int global_counter = 0;

void TestAsyncInjection(void);

int main(void)
{
	TestAsyncInjection();	

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

int TestFunc(void)
{
	std::cout << ++global_counter << std::endl;
	return 3 == global_counter;
}

void TestAsyncInjection(void)
{
	AsyncInjection::Create(std::function<bool()>(TestFunc),2);

	sleep(6);
}