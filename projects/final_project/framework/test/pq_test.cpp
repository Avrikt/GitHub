/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Lihi Cohen   				    *
* Date: 13.08.2023						    	*
* Description: Test file for Priority Queue  	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <iostream>

#include "pq.hpp"
#include "utilities.hpp"

int main(void)
{
    dev::PQ<int> pq;

    pq.push(5);
    pq.push(10);
    pq.push(2);

    TEST(10 == pq.front(), PQ);
    
    pq.pop();
     
    TEST(5 == pq.front(), PQ);

    return 0;
}