/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC Semaphore - Ping           	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /* printf */
#include <semaphore.h>  /* semaphore */
#include <fcntl.h>      /* O_CREAT */
#include <unistd.h>     /* sleep */

int main(void)
{
    int reps = 0;
    sem_t *sem_ping = sem_open("ping_sem", O_CREAT, 0644, 0);
    sem_t *sem_pong = sem_open("pong_sem", O_CREAT, 0644, 0);

    for(; reps < 5; ++reps)
    {
        printf("\x1B[1;33mPing\x1B[0m\n");
        sem_post(sem_pong);
        sleep(1);
        sem_wait(sem_ping);
    }

    sem_close(sem_ping);
    sem_close(sem_pong);

    return 0;
}