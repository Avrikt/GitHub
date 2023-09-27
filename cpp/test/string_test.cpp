/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai  				    *
* Date: 14.06.2023						    	*
* Description: Test file for Simple String      *
* Version: 2.1                                  *
* Status: Approved                              *
*************************************************/

#include <iostream>

#include "string.hpp"

using namespace dev;

int main(void)
{
    String s1("Michael Jordan");
    String s2(s1);
    String s3;
    String s4("Lebron James");


    cout << "Default Ctor" << endl;
    cout << "String: " << s1.Cstr() << endl;
    cout << "Length: " << s1.Length() << endl;
    

    cout << endl << "CCtor" << endl;
    cout << "String: " << s2.Cstr() << endl;
    cout << "Length: " << s2.Length() << endl;

    cout << endl << "Copy assignment" << endl;
    cout << "Before assignment: " << s3.Cstr() << endl;
    s3 = s2;
    cout << "After assignment: " << s3.Cstr() << endl;
    cout << "Length: " << s3.Length() << endl;
    
    cout << boolalpha;
    cout << "\ns1 == s2: " << (s1 == s2) << endl;
    cout << "\ns1 > s2: " << (s1 > s2) << endl;
    cout << "\ns1 < s2: " << (s1 < s2) << endl;

    cout << "\nLebron James < Michael Jordan: " << (s4 < s1) << endl;
    
    cout << s1 << endl;
    
    s1 = "123";
    
    cout << "\ns1 == \"123\": " << (s1 == "123") << endl;
    cout << "\n\"123\" == s1: " << ("123" == s1) << endl;

    s4[2] = 'Z';
    cout << s4 << endl;
    
    return 0;
}