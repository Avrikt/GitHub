#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define ERROR (-1)

void Pong(void);

void PongHandler(int sig);
void PingHandler(int sig);

int main(void)
{
    Pong();

    return 0;
}

void Pong(void)
{
   signal(SIGUSR1, PingHandler);
   signal(SIGUSR2, PongHandler);

    while(1)
    {
        sleep(1);
        kill(getppid(), SIGUSR2);
        pause();
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