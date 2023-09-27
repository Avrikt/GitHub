#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "fsq_5.h"

#define THREADS (5)
#define Q_SIZE (5)

void Ex5(void);

void *Producers(void *arg);

void *Consumers(void *arg);

fsq_t *fsq = NULL;

pthread_mutex_t prod_lock;
pthread_mutex_t cons_lock;

sem_t sem, sem_max;

int main(void)
{ 
    Ex5();
    
    return 0;
}

void Ex5(void)
{
    pthread_t t1[THREADS], t2[THREADS];
    size_t i = 0;
    fsq = FSQCreate(Q_SIZE);

    pthread_mutex_init(&prod_lock, NULL);
    pthread_mutex_init(&cons_lock, NULL);
    
    sem_init(&sem, 0, 0);
    sem_init(&sem_max, 0 , Q_SIZE);

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
    
    FSQDestroy(fsq);
    sem_close(&sem);
    sem_close(&sem_max);
    pthread_mutex_destroy(&prod_lock);
    pthread_mutex_destroy(&cons_lock);
}

void *Producers(void *arg)
{
    size_t i = 0;

    pthread_mutex_lock(&prod_lock);
    sem_wait(&sem_max);
    
    FSQEnqueue(fsq, *(int *)arg);
    printf("Message produced: %d\n", *(int *)arg);
    sem_post(&sem);
    
    pthread_mutex_unlock(&prod_lock);

    return NULL; 
}

void *Consumers(void *arg)
{
    size_t i = 0;
    int data = 0;

    sem_wait(&sem);
    pthread_mutex_lock(&cons_lock);
    
    data = FSQPeek(fsq);
    printf("Message consumed: %d\n", data);
    FSQDequeue(fsq);
    
    sem_post(&sem_max);
    pthread_mutex_unlock(&cons_lock);
 
    return NULL;
}