#include <stdio.h>
#include <stddef.h>
#include <pthread.h> /*spinlock*/
#include <stdatomic.h>
#include <unistd.h> /*sleep*/

#define BUFFER (1)
#define REPS (10)
#define EMPTY (0)
#define FULL (1)

static void Spinlock(void);

void *Producer(void *arg);

void *Consumer(void *arg);

struct message
{
    int text[BUFFER];
    int is_full;
}message;

int main(void)
{
    Spinlock();

    return 0;
}

static void Spinlock(void)
{
    pthread_t prod_th, cons_th;
    message.is_full = EMPTY;

    pthread_create(&prod_th, NULL, Producer, NULL);
    pthread_create(&cons_th, NULL, Consumer, NULL);

    pthread_join(prod_th, NULL);
    pthread_join(cons_th, NULL);

}

/*Producer*/
void *Producer(void *arg)
{
    size_t i = 0, j = 0;
    
    for (i = 0; i < REPS; ++i)
    {
        sleep(1);
        while (message.is_full != EMPTY);
        for (j = 0; j < BUFFER; ++j)
        {
            message.text[j] = i;
            printf("Message produced: %d\n", message.text[j]);
        }
        message.is_full = FULL;
    }

    return NULL;
}

/*Consumer*/
void *Consumer(void *arg)
{
    size_t i = 0, j = 0;
    
    for (i = 0; i < REPS; ++i)
    {    
        while (message.is_full == EMPTY);
        for (j = 0; j < BUFFER; ++j)
        {
            printf("Message consumed: %d\n", message.text[j]);
        }
        message.is_full = EMPTY;
    }
    
    return NULL;
}