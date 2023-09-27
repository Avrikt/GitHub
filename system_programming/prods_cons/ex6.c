#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define CONSUMERS (4)
#define PRODUCTS (sizeof(messages)/ sizeof(messages[0]))

void Ex6(void);

void *Producers(void *arg);

void *Consumers(void *arg);

sem_t sem;
pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;;

int cond_count = 0;

int messages[] = {1, 1 ,2 , 3, 5, 8, 13, 21, 34};

int buffer = 0; 

int main(void)
{ 
    Ex6();
    
    return 0;
}

void Ex6(void)
{
    pthread_t t1, t2[CONSUMERS];
    size_t i = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    sem_init(&sem, 0, 0);

    pthread_create(&t1, NULL, Producers, NULL);
    
    for (i = 0; i < CONSUMERS; ++i)
    {
        pthread_create(&t2[i], NULL, Consumers, NULL);
    }

    pthread_join(t1, NULL);

    for (i = 0; i < CONSUMERS; ++i)
    {
        pthread_join(t2[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    sem_close(&sem);
}

void *Producers(void *arg)
{
    size_t i = 0;
   
    for (i = 0; i < PRODUCTS; ++i)
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);

        printf("\x1b[32mAll messages consumed\033[0m\n\n");
    }

    return NULL; 
}

void *Consumers(void *arg)
{
    size_t i = 0;
    
    for (i = 0; i < PRODUCTS; ++i)
    {
        sem_wait(&sem);
        pthread_mutex_lock(&mutex);
        ++cond_count; 
        printf("Message consumed: %d\n", buffer);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}