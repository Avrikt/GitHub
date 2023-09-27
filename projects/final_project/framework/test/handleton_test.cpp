/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Makrov						*
* Date: 23.08.2023							    *
* Description: Test file for Handleton  		*
* Version: 1.1                              	*
* Status: Approved                            	*
*************************************************/
#include <iostream> // std::cerr, std::string
#include <dlfcn.h>  // dlopen

#include "handleton.hpp"
#include "transport.hpp"
#include "factory.hpp"

using namespace dev;

int main(void)
{
    Factory<ITransport, std::string> *factory = Handleton<Factory<ITransport, std::string>>::GetInstance();

    void *handle = dlopen("./lib/libhandleton_plugin.so", RTLD_LAZY);
    if(nullptr == handle)
    {
        std::cerr << "dlopen ";
    }

    ITransport *t1 = factory->Create("Truck");
    ITransport *t2 = factory->Create("Ship");
    ITransport *t3 = factory->Create("Car");

    t1->Drive();
    t2->Drive();
    t3->Drive();

    delete t1;
    delete t2;
    delete t3;
    
    return 0;
}