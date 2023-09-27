/********************
Developer: Avri Kehat
Reviewed by: Noam Gazit
Date: 12.02.23
*********************/

#include <stdio.h> /*perror*/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include "queue.h"
#include "linked_list.h"

struct queue
{
    sll_t *queue;
};

struct node
{
    void *data;
    node_t *next;
};

struct SingleLinkedList
{
	node_t *head;
	node_t *tail;
};

queue_t *QueueCreate(void)
{
	queue_t *new_queue = (queue_t *) malloc(sizeof(queue_t));
	
	if (NULL == new_queue)
	{
		perror("Error");
	}
	else 
	{	
		new_queue->queue = SLLCreate();
		if (NULL == new_queue->queue)
		{
			free(new_queue);
		}
	}
	
	return new_queue;
}

void QueueDestroy(queue_t *queue)
{
	assert (NULL != queue);
	SLLDestroy(queue->queue);
	free(queue);
}

int QueueEnqueue(queue_t *queue, void *data)
{
	assert (NULL != queue);
	assert (NULL != data);
	return !((queue_t *)queue->queue->tail == (queue_t *)SLLInsertBefore(queue->queue->tail, data));
}

void QueueDequeue(queue_t *queue)
{
	iterator_t tmp_iter = NULL;
	assert (NULL != queue);
	
	tmp_iter = queue->queue->head;
	queue->queue->head = queue->queue->head->next;

	free(tmp_iter);
}

void *QueuePeek(const queue_t *queue)
{
	assert (NULL != queue);
	return SLLGetData(queue->queue->head);
}

int QueueIsEmpty(const queue_t *queue)
{
	assert (NULL != queue);
	return SLLIsEmpty(queue->queue);
}

size_t QueueSize(const queue_t *queue)
{
	assert (NULL != queue);
	return SLLCount(queue->queue);
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
	assert (NULL != dest);
	assert (NULL != src);
	
	SLLAppend(dest->queue, src->queue);
	return dest;
}
