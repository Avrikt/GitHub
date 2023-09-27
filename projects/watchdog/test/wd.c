/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nir Davidovitch				    *
* Date: 22.05.2023						    	*
* Description: Source file for shared object	*
*************************************************/

#include <stdio.h>
#include <stddef.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "wd.h"
#include "utilities.h"

extern sig_atomic_t wd_flag;

int main(int argc, char *argv[])
{
    wd_flag = 1;
    size_t thresh = 0;
    
    if (NULL != getenv("WD_THRESH"))
    {
        thresh = atoi(getenv("WD_THRESH"));
    }

    kill(getppid(), SIGCONT);
    
    HEADER(Watchdog);
    TEST(0 == WDStart(argc, argv, thresh), WDStart);

    return 0;
}
