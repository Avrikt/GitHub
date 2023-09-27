/********************
Developer: Avri Kehat
Reviewed by:
Date: xx.xx.23
*********************/

#include <stdio.h> /*printf, size_t*/
#include <assert.h> /*assert*/
#include "queue.h"
#include "linked_list.h"

#define GRN ("\x1B[0;32m")
#define REDB ("\x1B[1;31m")
#define RESET ("\x1B[0m")
#define TEST_RESULT(FLAG) (0 == FLAG) ? printf("\n%sSuccess\n\n%s", GRN, RESET) \
								 : (printf("\n%sFailure, line %d\n\n%s", REDB, __LINE__, RESET))

static void TestQueue(void);
static int TestQueueIsEmpty(queue_t *queue);
static int TestQueueSize(queue_t *queue, size_t size);
static int TestEnqueue(queue_t *queue);
static int TestQueueDequeue(queue_t *queue);
static int TestQueuePeek(queue_t *queue, const int num);
static int TestQueueAppend(queue_t *dest, queue_t *src);

int main(void)
{
	TestQueue();
	return 0;
}


static void TestQueue(void)
{
	queue_t *new_queue = QueueCreate();
	queue_t *dest_queue = QueueCreate();
	
	int test_flag = 0;
	int num1 = 9;
	int num2 = 13;
	
	test_flag += TestQueueIsEmpty(new_queue);
	printf("QueueIsEmpty test:");
	TEST_RESULT(test_flag);
		
	test_flag += TestQueueSize(new_queue, 0);
	printf("QueueSize test:");
	TEST_RESULT(test_flag);
	
	test_flag += TestEnqueue(new_queue);
	printf("ALl tests of QueueEnqueue:");
	TEST_RESULT(test_flag);
	
	test_flag += TestQueueDequeue(new_queue);
	printf("QueueDequeue test:");
	TEST_RESULT(test_flag);

	test_flag += QueueEnqueue(dest_queue, &num1);
	test_flag += QueueEnqueue(dest_queue, &num2);
	
	test_flag += QueueEnqueue(new_queue, &num1);
	test_flag += QueueEnqueue(new_queue, &num2);
	
	test_flag = TestQueueAppend(dest_queue, new_queue);
	printf("QueueAppend test:");
	TEST_RESULT(test_flag);
	
	QueueDestroy(new_queue);
	QueueDestroy(dest_queue);
}

static int TestQueueIsEmpty(queue_t *queue)
{
	return !(QueueIsEmpty(queue));
}

static int TestQueueSize(queue_t *queue, size_t size)
{
	return !(size == QueueSize(queue));
}

static int TestEnqueue(queue_t *queue)
{
	int test_flag = 0;
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	
	test_flag = QueueEnqueue(queue, &num1);
	printf("QueueEnqueue test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestQueueSize(queue, 1);
	printf("QueueSize test to size of 1:");
	TEST_RESULT(test_flag);
	
	test_flag = TestQueuePeek(queue, num1);
	printf("QueuePeek test for value of %d:", num1);
	TEST_RESULT(test_flag);
	
	QueueEnqueue(queue, &num2);
	QueueEnqueue(queue, &num3);
	QueueEnqueue(queue, &num4);
	QueueEnqueue(queue, &num5);
	
	test_flag = TestQueueSize(queue, 5);
	printf("QueueSize test to size of 5:");
	TEST_RESULT(test_flag);

	test_flag = TestQueuePeek(queue, num1);
	printf("QueuePeek test for value of %d:", num1);
	TEST_RESULT(test_flag);

	return test_flag;
}

static int TestQueueDequeue(queue_t *queue)
{
	int test_flag = 0;
	
	QueueDequeue(queue);

	test_flag = TestQueueSize(queue, 4);
	printf("QueueSize test to size of 4:");
	TEST_RESULT(test_flag);

	QueueDequeue(queue);
	
	test_flag = TestQueueSize(queue, 3);
	printf("QueueSize test to size of 3:");
	TEST_RESULT(test_flag);

	QueueDequeue(queue);
	QueueDequeue(queue);
	QueueDequeue(queue);

	test_flag = TestQueueSize(queue, 0);
	printf("QueueSize test to size of 0:");
	TEST_RESULT(test_flag);
	
	test_flag = TestQueueIsEmpty(queue);
	printf("QueueIsEmpty test:");
	TEST_RESULT(test_flag);
	
	return test_flag;
}

static int TestQueuePeek(queue_t *queue, int num)
{
	return !(num == *(int *)QueuePeek(queue));
}	
	
static int TestQueueAppend(queue_t *dest, queue_t *src)
{
	size_t total_size = 0;
	
	assert (NULL != dest);
	assert (NULL != src);
	
	total_size = (QueueSize(dest)) + (QueueSize(src));
	
	dest = QueueAppend(dest, src);
	
	return !(QueueSize(dest) == total_size);
}

