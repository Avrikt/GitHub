/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Message queue - P3         	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf*/
#include <sys/msg.h>    /*struct msqid_ds ,msgget, msgctl, msgsnd, ftok*/
#include <stdlib.h>     /*exit*/

#define BUF 100
struct
{
    long msg_type;
    char msg[BUF];
}message = {0};

int main(void)
{
    struct msqid_ds msq_stat;
    int msqid = 0;
    const char *path = "/home/avri/git/system_programming/ipc/msg_1.c";
    int proj_id = 42;
    
    key_t key = ftok(path, proj_id);

    if(-1 == (msqid = msgget(key, 0666 | IPC_CREAT )))
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

    if(-1 == (msgrcv(msqid, &message, sizeof(message.msg), 0, IPC_NOWAIT)))
    {
        perror("msgrcv\n");
        msgctl(msqid, IPC_RMID, 0);
        exit(1);
    }

    printf("Message recieved: %s\n", message.msg);

    if(1 == msq_stat.msg_qnum)
    {
        printf("Message queue empty\n");
        msgctl(msqid, IPC_RMID, 0);
    }

    return 0;
}