/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai  				    *
* Date: 14.06.2023						    	*
* Description: Source file for Simple String	*
* Version: 2.1                                  *
* Status: Approved                              *
*************************************************/

#include <iostream> //ostream, cout
#include <cstring>  //strlen, strcpy, strcmp

#include "string.hpp"


namespace dev
{

inline void AssignString(char* &dest, const char *src);

String::String(const char *s_) : m_str(NULL)
{
    AssignString(m_str,s_);
}

String::String(const String& other_)
{
    AssignString(m_str, other_.m_str);
}

String::~String()
{
    delete[] m_str;
}

String& String::operator=(const String& other_)
{
    if(this != &other_)
    {
        delete[] m_str;
        AssignString(m_str , other_.m_str);
    }

    return *this;
}

char& String::operator[](size_t i)
{
    return m_str[i];
}

ostream& operator<<(ostream& os_, const String& o_)
{
    return os_ << o_.Cstr();
}

bool operator==(const String& lhs_, const String& rhs_)
{
    return (0 == strcmp(lhs_.Cstr(), rhs_.Cstr()));
}

bool operator<(const String& lhs_, const String& rhs_)
{
    return (0 > strcmp(lhs_.Cstr(), rhs_.Cstr()));
}

bool operator>(const String& lhs_, const String& rhs_)
{
    return (0 < strcmp(lhs_.Cstr(), rhs_.Cstr()));
}

size_t String::Length() const
{
    return strlen(m_str);
}

const char *String::Cstr() const
{
    return m_str;
}

inline void AssignString(char* &dest, const char *src)
{
    dest = new char[strlen(src) + 1];
    strcpy(dest, src);
}
}