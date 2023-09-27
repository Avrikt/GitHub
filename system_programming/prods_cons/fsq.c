#include <assert.h>
#include <stdlib.h>

#include "fsq.h"

enum status
{
    SUCCESS = 0,
    QUEUE_IS_FULL = -1,
    QUEUE_IS_EMPTY = -2
};

struct fsq
{
    int *array;
    size_t max_size;
    int size;
};


fsq_t *FSQCreate(size_t size)
{
    fsq_t *fsq = (fsq_t *)malloc(sizeof(fsq_t));
    if (NULL == fsq)
    {
        return NULL;
    }

    fsq->array = (int *)malloc(sizeof(int) * size);
    if (NULL == fsq->array)
    {
        free(fsq);
        return NULL;
    }
    
    fsq->max_size = size;
    fsq->size = 0;

    return fsq;    
}

void FSQDestroy(fsq_t *q)
{
    free(q->array);
    free(q);
}

int FSQEnqueue(fsq_t *q, int input)
{
    if (FSQIsFull(q))
    {
        return QUEUE_IS_FULL;
    }

    *(q->array + q->size) = input;
    ++q->size;

    return SUCCESS;
}

int FSQDequeue(fsq_t *q)
{
    size_t i = 0;

    if(FSQIsEmpty(q))
    {
        return QUEUE_IS_EMPTY;
    }

    for (i = 0; i < (q->size - 1); ++i)
    {
        q->array[i] = q->array[i + 1];
    }
    --q->size;

    return SUCCESS;
}

int FSQPeek(fsq_t *q)
{
    if(FSQIsEmpty(q))
    {
        return PEEK_ERROR;
    }

    return (q->array[0]);
}

int FSQIsEmpty(fsq_t *q)
{
    assert(NULL != q);

    return (0 == q->size);
}

int FSQIsFull(fsq_t *q)
{
    assert(NULL != q);

    return (q->max_size == q->size);
}