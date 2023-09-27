#include<stdio.h>       /*NULL*/
#include<sys/types.h>   /*pid_t*/
#include<sys/wait.h>    /*wait*/
#include <unistd.h>     /*sleep pause*/
#include <sys/types.h>  /*kill*/
#include <signal.h>     /*SIGUSR1 SIGUSR2*/
#include <stdlib.h>

void Parent(int pid);

void Child(int fd);

int main(int argc, char *argv[])
{
    pid_t pid = atoi(argv[1]);

    Parent(pid);

    return 0;
}

void Parent(int pid)
{
    signal(SIGUSR2, Child);

    while (1)
    {
        sleep(1);
        kill(pid, SIGUSR1);
        pause();
    }
}

void Child(int fd)
{
    (void) fd;
    printf("Pong\n");
}