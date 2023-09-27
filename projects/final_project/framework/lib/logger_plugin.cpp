/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Aaron Sarfaty					*
* Date: 29.08.2023							    *
* Description: Plugin for Logger            	*
* Version: 1.0                                  *
* Status: Approved                             	*
*************************************************/

#include "handleton.hpp"
#include "logger.hpp"

const std::string LOG_NAME = "./logger.txt";

void MyCtor(void) __attribute__((constructor));
void MyCtor(void)
{
    dev::Logger *log = dev::Handleton<dev::Logger, std::string>::GetInstance(LOG_NAME);
    
    log->LogMessage("Plugin Log file");
}