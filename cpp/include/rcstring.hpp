/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Egor Markov   				    *
* Date: 26.06.2023						    	*
* Description: Header file for RCString     	*
* Version: 1.3                                  *
* Status: Approved                              *
*************************************************/

#ifndef MY_PROJECT_RCSTRING_HPP
#define MY_PROJECT_RCSTRING_HPP

#if cplusplus >= 201103L 
// noexcept is defined, do nothing
#else
#define noexcept throw()
#endif //#if cplusplus >= 201103L 

#include <assert.h>
#include <algorithm> //swap()
#include <cassert>   //assert
#include <cstddef>   //size_t

namespace dev
{

class RCString
{
public: 
    RCString(const char* s_ = ""); //Non explicit on purpose
    ~RCString();
    RCString(const RCString& other_);
    RCString& operator=(const RCString& other_);
    
    inline const char *ToCStr() const;
    inline size_t Length() const;
    inline bool IsShared() const;
    inline char& operator[](size_t indx_);
    inline const char& operator[](size_t indx_) const;

private:
    struct m_rcString_t
    {
        
        size_t counter;
        char data[1];
    };

    m_rcString_t *m_sptr;
};

/************************************ Forward Declarations *******************************************/
inline bool operator>(const RCString& lhs_, const RCString& rhs_);
inline bool operator<(const RCString& lhs_, const RCString& rhs_);
inline bool operator==(const RCString& lhs_, const RCString& rhs_);
inline bool operator!=(const RCString& lhs_, const RCString& rhs_);


/************************************ Member Function Definitions *******************************************/

inline const char *RCString::ToCStr() const
{
    return m_sptr->data;
}

inline size_t RCString::Length() const
{
    return strlen(m_sptr->data);
}

inline bool RCString::IsShared() const
{
    return (1 < m_sptr->counter);
}

inline const char& RCString::operator[](size_t indx_) const
{
    assert(indx_ <= this->Length());

    return m_sptr->data[indx_];
}

inline char& RCString::operator[](size_t indx_)
{
    assert(indx_ <= this->Length());

    RCString tmp(this->ToCStr());
    std::swap(m_sptr, tmp.m_sptr);
    
    return m_sptr->data[indx_];
}

/************************************ Function Definitions *******************************************/
inline bool operator==(const RCString& lhs_, const RCString& rhs_)
{
    return (0 == strcmp(lhs_.ToCStr(), rhs_.ToCStr()));
}

inline bool operator!=(const RCString& lhs_, const RCString& rhs_)
{
    return !(lhs_ == rhs_);
}

inline bool operator>(const RCString& lhs_, const RCString& rhs_)
{
    return (0 < strcmp(lhs_.ToCStr(), rhs_.ToCStr()));
}

inline bool operator<(const RCString& lhs_, const RCString& rhs_)
{
    return (!(lhs_ > rhs_)) && (!(lhs_ == rhs_));
}

} //namespace

#endif //MY_PROJECT_RCSTRING_HPP