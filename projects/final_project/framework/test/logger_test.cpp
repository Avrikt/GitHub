/************************************************
* Developer: Avri Kehat							*
* Reviewed by: Aaron Sarfaty					*
* Date: 29.08.2023								*
* Description: Test file for Logger				*
* Version: 1.0                              	*
* Status: Approved                             	*
*************************************************/

/************************************LIBRARIES**************************************************/
#include <dlfcn.h> // dlopen, RTLD_LAZY 

#include "handleton.hpp"
#include "logger.hpp"
#include "utilities.hpp"

const std::string LOG_NAME = "./logger.txt";

using namespace dev;

int main(void)
{
	Logger *log = Handleton<Logger, std::string>::GetInstance(LOG_NAME);
	
	void *handle = dlopen("./lib/liblogger_plugin.so", RTLD_LAZY);
	if(!handle)
	{
		std::cerr << "Dlopen ";
		return utils::FAILURE;
	} 

	log->LogMessage("Test from logger_test");

	dlclose(handle);
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
