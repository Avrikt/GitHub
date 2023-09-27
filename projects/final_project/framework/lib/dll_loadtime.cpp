/************************************************
 Developer: Avri Kehat						    *
* Reviewed by: Tomer Savorai					*
* Date: 05.09.2023								*
* Description: Loaded Shared Object DllLoader  	*
* Version: 1.0                                  *
* Status: Approved                             	*
*************************************************/

#define I_AM_SHARED_LIBRARY
#include "handleton.hpp"
#include "dll_loader.hpp"
#include "command.hpp"
#include "factory.hpp"

template class dev::Handleton<dev::DLLLoader, std::string>;
template class dev::Handleton<dev::Factory<dev::ICommand, std::string>>;