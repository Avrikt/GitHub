#ifndef __UTILITIES_HPP_
#define __UTILITIES_HPP_

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <cstdio>

namespace utils
{
/************************************************************************************************
*									   Function success											*
*************************************************************************************************/

constexpr int SUCCESS = 0;
constexpr int FAILURE = 1;


/************************************************************************************************
*										Identifiers												*
*************************************************************************************************/

#define AND       &&
#define OR        ||
#define NOT       !
#define NOTEQUALS !=
#define EQUALS    ==
#define IS        =

/************************************************************************************************
*										  Colors												*
*************************************************************************************************/
#define RED 	("\x1B[31m")
#define REDBOLD ("\x1B[1;31m")
#define CYN 	("\x1B[36m")
#define GRN 	("\x1B[0;32m")
#define YEL 	("\x1B[33m")
#define BLU 	("\x1B[34m")
#define MAG 	("\x1B[4;35m")
#define WHT 	("\x1B[37m")
#define NORMAL 	("\x1B[0m")

/************************************************************************************************
*										Test macros												*
*************************************************************************************************/

#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-20s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-20s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) 	((0) == (x)) ?\
						printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
						printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)

#define HEADER(func_name) (printf("\n\t\033[4;36m %s \033[0m\n", #func_name));
/***********************************************************************************************/

}
#endif
