/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Egor Makrov						*
* Date: 23.08.2023								*
* Description: Test file for singleton			*
* Version: 1.1                              	*
* Status: Approved                            	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <iostream>		//std::cout
#include <thread>		//std::thread

#include "singleton.hpp"

const int REPEATS = 20;
std::mutex mut;

class MyClass
{
public:
	MyClass() = default;
	void Print() 
	{
		std::unique_lock<std::mutex> lock(mut);
		std::cout << "Object address: " << this << std::endl;
	}
};

int main(void)
{
	std::thread th[REPEATS];
	
	for (size_t i = 0; i < REPEATS; i++)
	{
		MyClass *tmp = dev::Singleton<MyClass>::GetInstance();
		th[i] = std::thread([tmp] {tmp->Print();} );
	}

	for (size_t i = 0; i < REPEATS; i++)
	{
		th[i].join();
	}
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
