/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai  				    *
* Date: 14.06.2023						    	*
* Description: Header file for Simple String	*
* Version: 2.1                                  *
* Status: Approved                              *
*************************************************/

#ifndef MY_PROJECT_STRING_HPP
#define MY_PROJECT_STRING_HPP

#include <iosfwd> //ostream
#include <cstddef> //size_t

using namespace std;

namespace dev
{

class String
{
public:
    String(const char *s_ = "");
    String(const String& other_);
    ~String();
    String& operator=(const String& other_);
    char &operator[](size_t i);
    
    
    size_t Length() const;
    const char *Cstr() const;

private:
    char *m_str;
};

ostream& operator<<(ostream& os_, const String& o_);
bool operator==(const String& lhs_, const String& rhs_);
bool operator<(const String& lhs_, const String& rhs_);
bool operator>(const String& lhs_, const String& rhs_);

}

#endif
