#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define ERROR (-1)

void Ping(void);

void PingHandler(int sig);
void PongHandler(int sig);

int main(void)
{
    Ping();

    return 0;
}

void Ping(void)
{
    pid_t pid = 0;
    char *path[20] = {"./pong", NULL};

    signal(SIGUSR1, PingHandler);
    signal(SIGUSR2, PongHandler);;

    pid = fork();

    if(0 == pid)
    {
        execv(path[0], path);
    }

    if(ERROR == pid)
    {
        exit(1);
    }

    if(0 < pid)
    {
        while(1)
        {
            sleep(1);
            kill(pid, SIGUSR1);
            pause();
        }
    }
}

void PingHandler(int sig)
{
    printf("Ping\n");
}

void PongHandler(int sig)
{
    printf("Pong\n");
}