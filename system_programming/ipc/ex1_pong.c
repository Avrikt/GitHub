/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC Semaphore - Pong           	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /* printf */
#include <semaphore.h>  /* semaphore */
#include <unistd.h>     /* sleep */

int main(void)
{
    int reps = 0;
    sem_t *sem_ping = sem_open("ping_sem", 0);
    sem_t *sem_pong = sem_open("pong_sem", 0);

    for(; reps < 5; ++reps)
    {
        sem_wait(sem_pong);
        printf("\x1B[1;35mPong\x1B[0m\n");
        sleep(1);
        sem_post(sem_ping);
    }

    sem_close(sem_ping);
    sem_close(sem_pong);

    return 0;
}