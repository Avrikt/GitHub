/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Markov   				    *
* Date: 26.06.2023						    	*
* Description: Source file for RCString     	*
* Version: 1.3                                  *
* Status: Approved                              *
*************************************************/

#include <cstring>  //strlen, strcpy
#include <new>      //operator new(),

#include "rcstring.hpp"

namespace dev
{

RCString::RCString(const char* s_) : m_sptr(NULL)
{
    assert(NULL != s_);
    
    m_sptr = static_cast<m_rcString_t *>(operator new(offsetof(m_rcString_t , data[1]) + strlen(s_)));
    strcpy(m_sptr->data, s_);
    m_sptr->counter = 1;
}

RCString::~RCString()
{
    --m_sptr->counter;
    
    if(0 == m_sptr->counter)
    {
        delete m_sptr;
    }
}

RCString::RCString(const RCString& other_) : m_sptr(other_.m_sptr)
{
    ++m_sptr->counter;
}

RCString& RCString::operator=(const RCString& other_)
{
    ++other_.m_sptr->counter;

    if(1 == m_sptr->counter)
    {
        delete m_sptr;
    }
    else
    {
        --m_sptr->counter;
    }

    m_sptr = other_.m_sptr;

    return *this;
}
}