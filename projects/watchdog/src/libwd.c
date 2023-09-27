/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: Nir Davidovitch				    *
* Date: 22.05.2023						    	*
* Description: Source file for shared object	*
*************************************************/

/************************************LIBRARIES**************************************************/
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <time.h>       /*time_t, time() */
#include <signal.h>     /*SIGUSR1*/
#include <sys/types.h>  /*getppid()*/
#include <unistd.h>     /*kill()*/
#include <sys/wait.h>
#include <stdatomic.h>
#include <pthread.h>    /*pthread_create*/
#include <semaphore.h>
#include <fcntl.h>      /* O_CREAT */

#include "wd.h"
#include "../include/sched.h"
#include "utilities.h"

#ifndef NDEBUG
#define PRINTF_DEBUG(...) (printf(__VA_ARGS__))
#else
#define PRINTF_DEBUG(...)
#endif

#define PERMISSIONS (0666)
#define INIT_VAL (0)
#define T (2)

/********************************FORWARD DECLERATIONS*******************************************/
void *Thread(void *arg);

static void MMIHandler(int signum);

static void DNRHandler(int signum);

static int SignalTask(void *arg);

static int CheckThreshUP(void *thresh);

static int CheckThreshWD(void *thresh);

static void CleanUp(void);

sig_atomic_t wd_flag = 0;

atomic_size_t counter = 0;

typedef struct sched_wrap
{
    size_t thresh;
    char **argv;
    int argc;
}sched_wrap_t;

sem_t *sem = NULL;
pid_t pid = 0;
pthread_t thread_id = 0;
sch_t *sched;
sched_wrap_t *wrapper;
atomic_int stop_flag = 0;

/************************************Functions***************************************************/

int WDStart(int argc, char *argv[], size_t thresh)
{
    struct sigaction sa = {0};
    wrapper = (sched_wrap_t *)malloc(sizeof(sched_wrap_t));
    sched = SchCreate();
    char thresh_buf[10] = {0};
    
    if (NULL == sched || NULL == wrapper)
    {
        CleanUp();
        return FAILURE;
    }
    
    sa.sa_handler = MMIHandler;
    if (-1 == sigaction(SIGUSR1, &sa, NULL))
    {
        CleanUp();
    }
    if(SIG_ERR == signal(SIGUSR2, DNRHandler))
    {
        CleanUp();
    }

    wrapper->argc = argc;
    wrapper->argv = argv;
    wrapper->thresh = thresh;
    
    if(0 == wd_flag)
    {
        sem_unlink("wd_semaphore");
    }

    sem = sem_open("wd_semaphore", O_CREAT, PERMISSIONS, INIT_VAL);
    if(SEM_FAILED == sem)
    {
        CleanUp();
    }

    if(0 == wd_flag)
    {
        sprintf(thresh_buf, "%lu", thresh);
        setenv("WD_THRESH", thresh_buf, 0);
        pid = fork();

        if(0 == pid)
        {
            if(-1 == execvp("./bin/wd.out", argv))
            {
                CleanUp();
                return FAILURE;
            }
        }
        else if (0 < pid)
        {         
            SchAddTask(sched, SignalTask, NULL, time(NULL), T);
            SchAddTask(sched, CheckThreshUP, wrapper, time(NULL) + 3, T + 1);

            if (SUCCESS != pthread_create(&thread_id, NULL, Thread, NULL))
            {
                CleanUp();
                return FAILURE;
            }
        }
        else
        {
            CleanUp();
        }
    }
    else
    {
        pid = getppid();
       
        SchAddTask(sched, SignalTask, NULL, time(NULL), T);

        SchAddTask(sched, CheckThreshWD, wrapper, time(NULL) + 4, T + 1);

        sem_post(sem);

        if(SUCCESS != SchRun(sched))
        {
           CleanUp();
        }
    }
      
    return SUCCESS;
}

int WDStop(void)
{
    kill(pid, SIGUSR2);
    raise(SIGUSR2);
    wait(NULL);
    pthread_join(thread_id, NULL);
    sem_unlink("wd_semaphore");

    return SUCCESS;
}

/************************************ Static Functions *******************************************/

void *Thread(void *arg)
{
    (void)arg;
    
    sem_wait(sem);

    if (SUCCESS != SchRun(sched))
    {
        CleanUp();
    }

    return NULL;
}

static void MMIHandler(int signum)
{
    (void) signum;
    counter = 0;
}

static void DNRHandler(int signum)
{
    (void)signum;

    stop_flag = 1;
}

static int SignalTask(void *arg)
{  
    (void)arg;

    if(0 == stop_flag)
    {
        PRINTF_DEBUG("From pid: %d, To pid: %d, Counter: %ld \n", getpid(), pid, counter);
        kill(pid, SIGUSR1);
        ++counter;
    }
    else
    {
        return FAILURE;
    }

    return SUCCESS;
}

static int CheckThreshUP(void *thresh)
{
    sched_wrap_t *handler = (sched_wrap_t *)thresh;

    if (handler->thresh < counter)
    {
        pid = fork();
        if(0 == pid)
        {
            if(-1 == execvp("./bin/wd.out", handler->argv))
            {
                return FAILURE;
            }
        }
        else if (0 < pid)
        {
            raise(SIGSTOP);
            wait(NULL);
        }
        else
        {
            return FAILURE;
        }
    }
    
    return SUCCESS;
}

static int CheckThreshWD(void *thresh)
{
    sched_wrap_t *handler = (sched_wrap_t *)thresh;
    
    if(handler->thresh < counter)
    {
        if(-1 == execvp(handler->argv[0], handler->argv))
        {
            return FAILURE;
        }
    }

    return SUCCESS;
}

static void CleanUp(void)
{
    SchDestroy(sched);
    sem_close(sem);
    sem_unlink("wd_semaphore");
    unsetenv("WD_THRESH");
    free(wrapper);
}
