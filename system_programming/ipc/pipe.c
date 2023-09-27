/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Pipe                      	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf*/
#include <stdlib.h>     /*exit*/
#include <unistd.h>     /*open, close, read, write, sleep*/
#include <pthread.h>    /*pid_t, fork*/
#include <string.h>     /*strlen*/

#define REPS 5

int main(void)
{
    pid_t pid = 0;
    int i = 0;
    char buffer[100] = {0};
    const char *prnt_msg = "Ping";
    const char *chld_msg = "Pong";
    int prnt_pipe[2] = {0};
    int chld_pipe[2] = {0};
    
    if(-1 == pipe(prnt_pipe))
    {
        perror("Pipe\n");
        exit(1);
    }
    
    if(-1 == pipe(chld_pipe))
    {
        perror("Pipe\n");
        exit(1);
    }
    
    pid = fork();

    if(0 == pid)
    {
        close(chld_pipe[0]);
        close(prnt_pipe[1]);
        
        for(; i < REPS; ++i)
        {
            read(prnt_pipe[0], buffer, sizeof(buffer));
            printf("\x1B[1;35m%s\x1B[0m\n", buffer);
            sleep(1);
            write(chld_pipe[1], chld_msg, strlen(chld_msg) + 1);
        }
    }
    else if(0 < pid)
    {
        close(prnt_pipe[0]);
        close(chld_pipe[1]);
        for(; i < REPS; ++i)
        {
            sleep(1);
            write(prnt_pipe[1], prnt_msg, strlen(prnt_msg) + 1);
            sleep(1);
            read(chld_pipe[0], buffer, sizeof(buffer));
            printf("\x1B[1;33m%s\x1B[0m\n", buffer);
        }
    }
    else
    {
        perror("Fork\n");
        exit(1);
    }

    return 0;
}