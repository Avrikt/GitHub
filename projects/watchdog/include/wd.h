/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Nir Davidovitch				*
* Date: 22.05.2023							*
* Description: Header file for wd			*
*********************************************/

#ifndef MY_PROJECT_WD_H__
#define MY_PROJECT_WD_H__

#include <stddef.h>

/*
* WDStart Description:
*	Creates a "Watchdog" process. The watchdog tests the calling process for responsiveness and reboots the calling process if not responsive.
*
* @params:
*   argc - argc passed to calling process
*   argv - array of argv[] passed to calling process
*   thresh - threshold which defines how many unresponsive signals sent before rebooting the calling process.  
*
* @returns:
*	0 if successful. Otherwise will return a non-zero value.
*
* @Complexity
*    Time: O(n)
*    Space O(1)
*/
int WDStart(int argc, char *argv[], size_t thresh);

/*
* WDStop Description:
*	Terminates the "Watchdog" process in an orderly manner.
*
* @params:
*   none
*
* @returns:
*	0 if successful. Otherwise will return a non-zero value.
*
* @Complexity
*    Time: O(n)
*    Space O(1)
*/
int WDStop(void);

#endif