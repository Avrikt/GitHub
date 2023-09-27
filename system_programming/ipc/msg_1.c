/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Message queue - P1         	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf*/
#include <sys/msg.h>    /*struct msqid_ds ,msgget, msgctl, msgsnd, ftok*/
#include <stdlib.h>     /*exit*/

#define BUF 100
struct
{
    long m_type;
    char msg[BUF];
} message = {1 , "This is from the message queue"};

int main(void)
{
    int reps = 3;
    int msqid = 0;
    struct msqid_ds msq_stat;

    const char *path = "/home/avri/git/system_programming/ipc/msg_1.c";
    int proj_id = 42;

    key_t key = ftok(path, proj_id);
    
    if(-1 == (msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)))
    {
        perror("msgget\n");
        exit(1);
    }

    if(-1 == (msgctl(msqid, IPC_STAT, &msq_stat)))
    {
        perror("msgctl\n");
        msgctl(msqid, IPC_RMID, 0);
        exit(1);
    }

    for(;reps > 0; --reps)
    {
        if(-1 == msgsnd(msqid, &message, sizeof(message.msg), IPC_NOWAIT))
        {
            msgctl(msqid, IPC_RMID, 0);
            perror("msgsnd\n");
            exit(1);
        }
    }


    return 0;
}