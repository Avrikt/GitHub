/************************************************************
* Developer: Avri Kehat										*
* Reviewed by: David Hairapetian							*
* Date: 27.02.2023											*
* Description: Functions implementation of C Priority Queue *
*************************************************************/

#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "pq.h"
#include "sorted_list.h"

struct pq
{
	sol_t *sorted_list;
};

pq_t *PQCreate(int (*priority_func)(const void *lhs, const void *rhs))
{
	pq_t *new_list = NULL;
	assert (NULL != priority_func);

	new_list = (pq_t *) malloc(sizeof(pq_t));

	if (NULL == new_list)
	{
		return NULL;
	}

	new_list->sorted_list = SortedListCreate(priority_func);

	if (NULL == new_list->sorted_list)
	{
		free(new_list);
		return NULL;
	}

	return new_list;
}

void PQDestroy(pq_t *pq)
{
	assert (NULL != pq);

	SortedListDestroy(pq->sorted_list);
	free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
	assert(NULL != pq);

	return (1 == SortedListIsSameIter(SortedListEndIter(pq->sorted_list), SortedListInsert(pq->sorted_list, data)));
}

void PQDequeue(pq_t *pq)
{
	assert(NULL != pq);

	SortedListPopFront(pq->sorted_list);
}

void *PQPeek(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListGetData(SortedListBeginIter(pq->sorted_list));
}

int PQIsEmpty(const pq_t *pq)
{
	assert (NULL != pq);

	return SortedListIsEmpty(pq->sorted_list);
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);

	return SortedListSize(pq->sorted_list);
}

void *PQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), void *param)
{
	void *return_data = NULL;
	sol_iterator_t iter = {0};
	assert(NULL != pq);
	assert(NULL != is_match);

	iter = SortedListFindIf(SortedListBeginIter(pq->sorted_list), SortedListEndIter(pq->sorted_list), is_match, param);

	if (!SortedListIsSameIter(SortedListEndIter(pq->sorted_list), iter))
	{
		return_data = SortedListGetData(iter);
		SortedListRemove(iter);
	}

	return return_data;
}

void PQClear (pq_t *pq)
{
	assert (NULL != pq);

	while (!PQIsEmpty(pq))
	{
		PQDequeue(pq);
	}
}

pq_t *PQMerge(pq_t *pq_dest, pq_t *pq_src)
{
	assert (NULL != pq_dest);
	assert (NULL != pq_src);

	pq_dest->sorted_list = SortedListMerge(pq_dest->sorted_list, pq_src->sorted_list);

	return pq_dest;
}
