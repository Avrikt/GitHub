/***************************************************/

#ifndef MY_PROJECT_QUEUE_H__
#define MY_PROJECT_QUEUE_H__

#include <stddef.h> /*size_t*/

typedef struct queue queue_t;

/***************************************/

/*
* CreateQueue Description:
* Creates a queue.
*
* @Params:
* None
*
* @Returns:
* pointer to the created queue.
*
* @Complexity
* Time: O(1)
*/
queue_t *QueueCreate(void);


/*
* QueueDestroy Description:
* Destroys the given queue.
*
* @Params:
* queue - pointer to the queue to be destroyed.
* 	passing an invalid queue will lead to undefined behavior.
*
* @Returns:
* void
*
*
* @Complexity
* Time: O(n)
*/
void QueueDestroy(queue_t *queue);


/*
* QueueEnqueue Description:
* Adds an element to the end of the queue.
*
* @Params:
* queue - pointer to the queue.
*
* @Returns:
* returns 0 if the enqueue succeeded, or 1 if failed
*
* @Complexity
* Time: Amortized O(1)
*/
int QueueEnqueue(queue_t *queue, void *data);


/*
* QueueDequeue Description:
* Removes the element that was added first to the queue.
* 	removing an element from an empty queue will lead to undefined behavior.
*
* @Params:
* queue - pointer to the queue.
*
* @Returns:
* No return value.
*
* @Complexity
* Time: O(1)
*/
void QueueDequeue(queue_t *queue);


/*
* QueuePeek Description:
* Returns the value of the next element to be dequeued without dequeuing it.
*
* @Params:
* queue - pointer to the queue.
*	peeking at an empty queue will lead to undefined behavior.
*
* @Returns:
* Void * to data.
*
* @Complexity
* Time: O(1)
*/
void *QueuePeek(const queue_t *queue);


/*
* QueueIsEmpty Description:
* Checks if a given queue is empty.
*
* @Params:
* queue - pointer to the queue.
*
* @Returns:
* Returns 1 if queue is empty, 0 if not.
*
* @Complexity
* Time: O(1)
*/
int QueueIsEmpty(const queue_t *queue);


/*
* QueueSize Description:
* Returns number of elements in queue.
*
* @Params:
* queue - pointer to the queue.
*
* @Returns:
* Returns the number of elements in queue.
*
* @Complexity
* Time: O(n)
*/
size_t QueueSize(const queue_t *queue);


/*
* QueueAppend Description:
* Appends queue to another given queue.
*
* @Params:
* dest - pointer to the queue.
* src - Pointer to a queue to be appened.
*
* @Returns:
* 	returns a pointer to the dest queue (the joint queue)
*
* @Complexity
* Time: O(1)
*/
queue_t *QueueAppend(queue_t *dest, queue_t *src);


#endif
