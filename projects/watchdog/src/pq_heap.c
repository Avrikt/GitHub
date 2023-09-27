/********************************************
* Developer: Avri Kehat						*
* Reviewed by:								*
* Date: xx.xx.23							*
* Description: Source file for PQ Heap		*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "pq.h"
#include "heap.h"

/********************************FORWARD DECLERATIONS*******************************************/

struct pq
{
    heap_t *heap;
};
/************************************Functions***************************************************/

pq_t *PQCreate(int (*priority_func)(const void *lhs, const void *rhs))
{
    pq_t *new_queue = NULL;

    assert(NULL != priority_func);

    new_queue = (pq_t *)malloc(sizeof(pq_t));
    if (NULL == new_queue)
    {
        return NULL;
    }

    new_queue->heap = HeapCreate(priority_func);
    if (NULL == new_queue->heap)
    {
        free(new_queue);
        return NULL;
    }

    return new_queue;
}

void PQDestroy(pq_t *pq)
{
    HeapDestroy(pq->heap);
    free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
    assert(NULL != pq);

    return HeapPush(pq->heap, data);
}

void PQDequeue(pq_t *pq)
{
    assert(NULL != pq);

    HeapPop(pq->heap);
}

void *PQPeek(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapPeek((pq->heap));
}

int PQIsEmpty(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapIsEmpty(pq->heap);
}

size_t PQSize(const pq_t *pq)
{
    assert(NULL != pq);

    return HeapSize(pq->heap);
}

void *PQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), void *param)
{
    assert(NULL != pq);
    assert(NULL != is_match);

    return HeapRemove(pq->heap, is_match, param);
}

void PQClear(pq_t *pq)
{
    assert(NULL != pq);

    while(!PQIsEmpty(pq))
    {
        HeapPop(pq->heap);
    }
}
