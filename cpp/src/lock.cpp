/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Adir Ortal   				    *
* Date: 02.08.2023						    	*
* Description: Source file for Lock          	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/
#include <iostream>
#include <thread>
#include <mutex> 

#include "lock.hpp"

namespace dev
{
int syn_counter = 0;

void SyncCounter()
{
    static std::mutex mut;
    LockGuard<std::mutex> l1(mut);
    ++syn_counter;
    std::cout << syn_counter << " ";
}

void RunLock(void)
{  
    std::thread t1(SyncCounter);
    std::thread t2(SyncCounter);
    std::thread t3(SyncCounter);
    std::thread t4(SyncCounter);
    std::thread t5(SyncCounter);
    std::thread t6(SyncCounter);

    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    std::cout << std::endl;
}

}