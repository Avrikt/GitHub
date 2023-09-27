/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai  				    *
* Date: 20.06.2023						    	*
* Description: Test file for Complex          	*
* Version: 2.1                                  *
* Status: Aprroved                              *
*************************************************/
#include <iostream>
#include <ctime>

#include "complex.hpp"
#include "utilities.hpp"

const size_t REPEATS(10000000);

using namespace dev;

int main(void)
{
     HEADER(Default Ctor);
    Complex c1;
    std::cout << c1 << std::endl; 
    
    HEADER(Paramatized Ctor);
    Complex c2(1.1, 2.2);
    Complex c3(3.3, 4.4);
    
    std::cout << "c3: " << c3 << std::endl;

    HEADER('<<' OP);
    std::cout << "c2: " << c2 << std::endl;

    HEADER('+' OP);
    c1 = c2 + c3;
    std::cout << "c2: " << c2 << std::endl;
    std::cout << "c3: " << c3 << std::endl;
    std::cout << "\tc1 = c2 + c3;" << std::endl;
    std::cout << "c1: " << c1 << std::endl;

    HEADER('-' OP);
    c1 = c3 - c2;
    std::cout << "c3: " << c3 << std::endl;
    std::cout << "c2: " << c2 << std::endl;
    std::cout << "\tc1 = c3 - c2;" << std::endl;
    std::cout << "c1: " << c1 << std::endl;
    
    HEADER('+=' OP);
    std::cout << "c1: " << c1 << std::endl;
    std::cout << "c3: " << c3 << std::endl;
    c1 += c3;
    std::cout << "\tc1 += c3;" << std::endl;
    std::cout << "c1: " << c1 << std::endl;

    HEADER('-=' OP);
    std::cout << "c1: " << c1 << std::endl;
    std::cout << "c2: " << c2 << std::endl;
    std::cout << "\tc1 -= c2;" << std::endl;
    c1 -= c2;
    std::cout << "c1: " << c1 << std::endl;


    Complex c4(c3);
    HEADER('==' OP);
    TEST(true == (c4 == c3), (c4 == c3));

    HEADER('!=' OP);
    TEST(true == (c1 != c3), (c1 != c3));

    HEADER('>>' OP);
    std::cout << "Insert two params: " << std::endl;
    std::cin >> c1;
    std::cout << "c1: " << c1 << std::endl;

    clock_t start = clock();

    for (size_t i = 0; i < REPEATS; ++i)
    {
        c1 = c2 + c3;
    }
    
    clock_t end = clock();
    
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    std::cout << "Execution time: " << duration << " seconds" << std::endl;
    return 0;
}