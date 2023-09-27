/********************************************
* Developer: Avri Kehat						*
* Reviewed by: David Hairapetian			*
* Date: 27.02.2023							*
* Description: Test file for priority queue	*
*********************************************/

/************************************TEST MACRO*************************************************/
#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-20s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-20s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) ((0) == (x)) ?\
printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)
/***********************************************************************************************/

#include <stdio.h> /*printf*/

#include "pq.h"

static void TestPQueue(void);
static int TestPQPeek(pq_t *pq, int num);
static int TestDequeue(pq_t *pq);
static int TestPQMerge(pq_t *pq_dest, pq_t *pq_src, int num);
static int TestPQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), int num);
static int TestPQClear(pq_t *pq);
int CmpInts(const void *num1, const void *num2);
int int_match(const void *num1, const void *num2);

/************************************TEST FUNCTIONS*********************************************/

int main(void)
{
	TestPQueue();

	return 0;
}

static void TestPQueue(void)
{
	int num1 = 13;
	int num2 = 2;
	int num3 = 5;
	int num4 = 3;
	int merge1 = 7;
	int merge2 = 1;
	int merge3 = 11;
	int merge4 = 18;

	pq_t *first_queue = PQCreate(&CmpInts);
	pq_t *second_queue = PQCreate(&CmpInts);

	TEST(1 == PQIsEmpty(first_queue), PQIsEmpty);

	TEST(0 == PQSize(first_queue), PQSize);

	TEST(0 == PQEnqueue(first_queue, &num1), PQEnqueue);

	TEST(0 == TestPQPeek(first_queue, num1), PQPeek);

	TEST(0 == PQEnqueue(first_queue, &num2), PQEnqueue);

	TEST(0 == TestPQPeek(first_queue, num2), PQPeek);

	PQEnqueue(first_queue, &num3);

	PQEnqueue(first_queue, &num4);

	TEST(4 == PQSize(first_queue), PQSize);

	TEST(0 == TestDequeue(first_queue), PQDequeue);

	PQEnqueue(second_queue, &merge1);
	PQEnqueue(second_queue, &merge2);
	PQEnqueue(second_queue, &merge3);
	PQEnqueue(second_queue, &merge4);

	TEST(0 == TestPQMerge(first_queue, second_queue, merge2), PQMerge);

	/******Check second list is empty after merge******/
	TEST(1 == PQIsEmpty(second_queue), PQIsEmpty);

	TEST(0 == TestPQErase(first_queue, &int_match, num1), PQErase);

	TEST(0 == TestPQClear(first_queue), PQClear);

	PQDestroy(first_queue);
	PQDestroy(second_queue);
}

static int TestPQPeek(pq_t *pq, int num)
{
	return !(num == *(int *) PQPeek(pq));
}

static int TestDequeue(pq_t *pq)
{
	int deq_num = 0;

	deq_num = *(int *)PQPeek(pq);
	PQDequeue(pq);

	return !(deq_num < *(int *)PQPeek(pq));
}

static int TestPQErase(pq_t *pq, int (*is_match)(const void *data, const void *param), int num)
{
	return !(num == *(int *)PQErase(pq, is_match, &num));
}

static int TestPQClear(pq_t *pq)
{
	PQClear(pq);

	return !(PQIsEmpty(pq));
}

static int TestPQMerge(pq_t *pq_dest, pq_t *pq_src, int num)
{
	PQMerge(pq_dest, pq_src);

	return TestPQPeek(pq_dest, num);
}

/************************************USER FUNCTIONS*************************************************/

int CmpInts(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}

int int_match(const void *num1, const void *num2)
{
	return (*(int *)num1 == *(int *)num2);
}
