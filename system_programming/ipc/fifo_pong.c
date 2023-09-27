/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Named Pipe - Pong           *
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <stdio.h>      /*printf, perror*/
#include <stdlib.h>     /*exit*/
#include <unistd.h>     /*open, close, read, write, sleep*/
#include <string.h>     /*strlen*/
#include <sys/stat.h>   /*mkfifo*/
#include <fcntl.h>      /*O_WRONLY, O_RDONLY*/

#define REPS 5
#define BUF 100

int main(void)
{
    const char *ping_path = "fifo_ping";
    const char *pong_path = "fifo_pong";
    const char *msg = "Pong";
    int w_fd = 0, r_fd = 0;
    char buffer[BUF] = {0};
    int i = 0;

    if(-1 == mkfifo(pong_path, 0644))
    {
        perror("mkfifo\n");
        exit(1);
    }

    r_fd = open(ping_path, O_RDONLY);
    w_fd = open(pong_path, O_WRONLY);

    for (; i < REPS; ++i)
    {
        read(r_fd, buffer, sizeof(buffer));
        printf("\x1B[1;33m%s\x1B[0m\n", buffer);
        write(w_fd, msg, strlen(msg)); 
    }
    
    close(w_fd);
    close(r_fd);

    unlink(pong_path);
    
    return 0;
}