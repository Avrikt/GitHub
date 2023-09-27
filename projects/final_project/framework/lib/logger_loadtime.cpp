/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Aaron Sarfaty					*
* Date: 29.08.2023							    *
* Description: Loaded Shared Object for Logger 	*
* Version: 1.0                                  *
* Status: Approved                             	*
*************************************************/

#define I_AM_SHARED_LIBRARY
#include "handleton.hpp"
#include "logger.hpp"

template class dev::Handleton<dev::Logger, std::string>;