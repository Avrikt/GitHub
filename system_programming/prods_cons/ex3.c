#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#include "dll.h"

#define THREADS (5)

static void Ex3(void);

void *Producers(void *arg);

void *Consumers(void *arg);

dll_t *dll = NULL;

pthread_mutex_t prod_lock;

sem_t *sem;

int main(void)
{
    Ex3();

    return 0;
}

static void Ex3(void)
{
    pthread_t t1[THREADS], t2[THREADS];
    size_t i = 0;
    int *arg = NULL;
    dll = DLLCreate();
    sem = sem_open("sem", O_CREAT, 0666, 0);
    
    pthread_mutex_init(&prod_lock, NULL);

    for (i = 0; i < THREADS; ++i)
    {
        pthread_create(&t1[i], NULL, Producers, (void *)&i);
        pthread_create(&t2[i], NULL, Consumers, NULL);
    }

    for (i = 0; i < THREADS; ++i)
    {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }
    
    DLLDestroy(dll);
    sem_close(sem);
    pthread_mutex_destroy(&prod_lock);
}

void *Producers(void *arg)
{
    size_t i = 0;
    int *data = NULL;

    pthread_mutex_lock(&prod_lock);
    
    data = malloc(sizeof(int));
    *data = *((int *)arg);
    DLLInsert(DLLEndIter(dll), data);
    printf("Message produced: %d\n", *data);
    sem_post(sem);
    
    pthread_mutex_unlock(&prod_lock);

    return NULL; 
}

void *Consumers(void *arg)
{
    size_t i = 0;
    int *data = NULL;

    sem_wait(sem);
    pthread_mutex_lock(&prod_lock);
    
    data = (int *)DLLPopFront(dll);
    printf("Message consumed: %d\n", *data);
    
    pthread_mutex_unlock(&prod_lock);
    free(data);

    return NULL;
}