/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Dave Hairapetian  			    *
* Date: 18.07.2023						    	*
* Description: Test file for Tree            	*
* Version: 1.2                                  *
* Status: Approved                              *
*************************************************/
#include <iostream>

#include "tree.hpp"

using namespace dev;

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <path>\n";
        return 1;
    }

    Tree tree(argv[1]);

    Tree *t2;
    t2 = tree.Clone();

    t2->Print();
    
    delete t2;
    return 0;
}