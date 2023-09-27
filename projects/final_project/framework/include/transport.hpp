/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai   				    *
* Date: 28.08.2023						    	*
* Description: Header file for Transport       	*
* Version: 1.0                                  *
* Status: Sent                                  *
*************************************************/

#ifndef MY_PROJECT_TRANSPORT_HPP
#define MY_PROJECT_TRANSPORT_HPP

namespace dev
{

class ITransport
{
public:
    virtual ~ITransport() = default;
    virtual void Drive() = 0;
};

}   //namespace dev

#endif // MY_PROJECT_TRANSPORT_HPP