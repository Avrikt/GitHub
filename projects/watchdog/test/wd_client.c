/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Nir Davidovitch				*
* Date: 22.05.2023							*
* Description: Test file for wd_client		*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "wd.h"
#include "utilities.h"
/*********************************GLOBAL VARRIABLES*********************************************/


/********************************FORWARD DECLERATIONS*******************************************/

int main(int argc, char *argv[])
{
	time_t start = time(NULL);
	
	HEADER(User Process);
	TEST(SUCCESS == WDStart(argc, argv, 4), WDStart);
	
	while(time(NULL) < start + 10);

	TEST(SUCCESS == WDStop(), WDStop);

	return 0;
}