#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void PingPong(void);
void Parent(int sig);
void Child(int sig);

int main(void)
{
    PingPong();

    return 0;
}

void PingPong(void)
{
    pid_t pid = 0;

    signal(SIGUSR1, Child);
    signal(SIGUSR2, Parent);

    pid = fork();
    
    alarm(10);

    if (0 < pid)
    {
        while (1)
        {
            sleep(1);
            kill(pid, SIGUSR2);
            pause();
        }
            
    }
    else if (0 == pid)
    {
        sleep(1);
        while (1)
        {
           sleep(1);
           kill(getppid(), SIGUSR1);
           pause();
        }
        
    }
    else
    {
        exit(1);
    }
}

void Parent(int sig)
{
    (void)sig;
    printf("\x1B[1;35mPing\x1B[0m\n");
}

void Child(int sig)
{
    (void)sig;
    printf("\x1B[1;37mPong\x1B[0m\n");
}