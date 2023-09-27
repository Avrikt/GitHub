#include<stdio.h>       /*NULL*/
#include<sys/types.h>   /*pid_t*/
#include<sys/wait.h>    /*wait*/
#include <unistd.h>     /*sleep pause*/
#include <sys/types.h>  /*kill*/
#include <signal.h>     /*sigaction struct, SIGUSR1, SIGUSR2,*/


void ParentHandle(int signum, siginfo_t *info, void *dummy);

int main(void)
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = ParentHandle;

    sigaction(SIGUSR1, &sa, NULL);
    printf("pid: %d\n", getpid());

    while(1)
    {
        sleep(1);
        pause();
    }

    return 0;
}

void ParentHandle(int signum, siginfo_t *info, void *dummy)
{
    printf("Ping\n");
    kill(info->si_pid, SIGUSR2);
}