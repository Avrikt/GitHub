/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Aviv Haklai   				    *
* Date: 28.08.2023						    	*
* Description: Loaded Shared Object Handleton  	*
* Version: 1.1                                  *
* Status: Approved                              *
*************************************************/

#define I_AM_SHARED_LIBRARY
#include "handleton.hpp"
#include "transport.hpp"
#include "factory.hpp"

template class dev::Handleton<dev::Factory<dev::ITransport, std::string>>;