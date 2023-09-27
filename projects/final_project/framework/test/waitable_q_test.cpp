/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai  				    *
* Date: 13.08.2023						    	*
* Description: Test file for Waitable Queue  	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "pq.hpp"
#include "waitable_q.hpp"
#include "utilities.hpp"

dev::WaitableQueue<dev::PQ<int>> queue;

const int THREADS(10);

void Producer(void);

void Consumer(void);

int main(void)
{
    boost::thread prod[THREADS];
    boost::thread cons[THREADS];

    srand(time(NULL));
    for (size_t i = 0; i < THREADS; i++)
    {
        prod[i] = boost::thread(Producer);
        cons[i] = boost::thread(Consumer);
    }

    for (size_t i = 0; i < THREADS; i++)
    {
        prod[i].join();
        cons[i].join();
    }
    
    return 0;
}

void Producer(void)
{
    int x = rand() % 100;
    queue.Push(x);
    sleep(1);
}

void Consumer(void)
{
    int *val = new int;
    queue.PopTimeout(*val, boost::chrono::milliseconds(1500));
    std::cout << *val << std::endl;
}