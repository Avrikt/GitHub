/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nitai Izkovich  				    *
* Date: 24.07.2023						    	*
* Description: IPC: Named Pipe - Ping          	*
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
    const char *msg = "Ping";
    int w_fd = 0, r_fd = 0;
    char buffer[BUF] = {0};
    int i = 0;

    if(-1 == mkfifo(ping_path, 0644))
    {
        perror("mkfifo\n");
        exit(1);
    }

    w_fd = open(ping_path, O_WRONLY);
    r_fd = open(pong_path, O_RDONLY);
    
    for (; i < REPS; ++i)
    {
        write(w_fd, msg, strlen(msg));
        sleep(1);
        read(r_fd, buffer, sizeof(buffer));
        printf("\x1B[1;35m%s\x1B[0m\n", buffer);
    } 
    
    close(w_fd);
    close(r_fd);

    unlink(ping_path);

    return 0;
}