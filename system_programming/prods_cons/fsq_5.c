#include <stdio.h>
#include <stdlib.h>

#include "fsq_5.h"

#define SUCCESS (0)


struct fsq
{
    size_t read;
    size_t write;
    size_t max_size;
    int *array;
};


fsq_t *FSQCreate(size_t size)
{
    fsq_t *q = (fsq_t *)malloc(sizeof(fsq_t));
    if (NULL == q)
    {
        return NULL;
    }

    q->array = (int *)malloc(sizeof(int) * size);
    if (NULL == q->array)
    {
        free(q);
        return NULL;
    }
     
     q->max_size = size;
     q->read = 0;
     q->write = 0;

     return q;
}

void FSQDestroy(fsq_t *q)
{
    free(q->array);
    free(q);
}

void FSQEnqueue(fsq_t *q, int input)
{
    *(q->array + q->write) = input;
    q->write = (q->write + 1) % q->max_size;
}

void FSQDequeue(fsq_t *q)
{
    q->read = (q->read + 1) % q->max_size;
}

int FSQPeek(fsq_t *q)
{
    return *(q->array + q->read);
}