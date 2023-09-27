/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Shared memory - P4         	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf*/
#include <sys/shm.h>    /*key_t, ftok, shmeg, IPC_CREAT, shmat, shmdt*/
#include <stdlib.h>     /*exit*/
#include <string.h>     /*strlen*/

#define SHM_BUF 100

int main(void)
{
    const char *path = "/home/avri/git/system_programming/ipc/msg_1.c";
    char *shmem = NULL;
    int proj_id = 42;
    int shmid = 0;
    key_t key = ftok(path, proj_id);

    if(-1 == (shmid = shmget(key, SHM_BUF, 0666 | IPC_CREAT)))
    {
        perror("Shmget\n");
        exit(1);        
    }

    if(NULL == (shmem = shmat(shmid, NULL, 0)))
    {
        perror("shmat\n");
        exit(1);
    }

    printf("%s\n", shmem);

    shmctl(shmid, IPC_RMID, NULL);

    shmdt(shmem);

    return 0;
}