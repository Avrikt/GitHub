#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "dll.h"

#define THREADS (5)

static void Ex2(void);

void *Producers(void *arg);

void *Consumers(void *arg);

dll_t *dll = NULL;
pthread_mutex_t prod_lock;

int main(void)
{
    Ex2();

    return 0;
}

static void Ex2(void)
{
    pthread_t t1[THREADS], t2[THREADS];
    size_t i = 0;
    int *arg = NULL;
    dll = DLLCreate();
    
    pthread_mutex_init(&prod_lock, NULL);

    for (i = 0; i < THREADS; ++i)
    {
        pthread_create(&t1[i], NULL, Producers, &i);
        pthread_create(&t2[i], NULL, Consumers, NULL);
    }

    for (i = 0; i < THREADS; ++i)
    {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }
    
    DLLDestroy(dll);

    pthread_mutex_destroy(&prod_lock);
}

void *Producers(void *arg)
{
    size_t i = 0;
    int *data = NULL;

    data = malloc(sizeof(int));
    *data = *(int *)arg;
    pthread_mutex_lock(&prod_lock);

    DLLInsert(DLLEndIter(dll), data);
    printf("Message produced: %d\n", *data);
    
    pthread_mutex_unlock(&prod_lock);

    return NULL; 
}

void *Consumers(void *arg)
{
    size_t i = 0;
    int *data = NULL;

    pthread_mutex_lock(&prod_lock);
    
    while (DLLIsEmpty(dll))
    {
        pthread_mutex_unlock(&prod_lock);
        sched_yield();
        pthread_mutex_lock(&prod_lock);
    }
    
    data = (int *)DLLPopFront(dll);
    printf("Message consumed: %d\n", *data);
    free(data);
    pthread_mutex_unlock(&prod_lock);

    return NULL;
}