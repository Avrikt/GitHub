/********************************************
* Developer: Avri Kehat						*
* Reviewed by: David Hairapetian			*
* Date: 27.02.2023							*
* Description: Test file for PQ heap		*
*********************************************/

#include "pq.h"
#include "utilities.h"

static void TestPQueue(void);
static int TestPQPeek(pq_t *pq, int num);
static int TestDequeue(pq_t *pq);
int CmpInts(const void *num1, const void *num2);
int IsMatch(const void *num1, const void *num2);

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
	int num3 = 15;
	int num4 = 3;
	int num5 = 7;
	int num6 = 1;
	int num7 = 11;
	int num8 = 18;

	pq_t *queue = PQCreate(CmpInts);
	HEADER(PQCreate);
	TEST(NULL != queue, PQCreate);
	TEST(TRUE == PQIsEmpty(queue), PQIsEmpty);
	TEST(0 == PQSize(queue), PQSize);

	HEADER(PQEnqueue);
	TEST(SUCCESS == PQEnqueue(queue, &num1), PQEnqueue);
	TEST(SUCCESS == PQEnqueue(queue, &num2), PQEnqueue);

	HEADER(PQPeek);
	TEST(SUCCESS == TestPQPeek(queue, num1), PQPeek);
	PQEnqueue(queue, &num3);
	TEST(SUCCESS == TestPQPeek(queue, num3), PQPeek);

	HEADER(PQSize);
	PQEnqueue(queue, &num4);
	TEST(4 == PQSize(queue), PQSize);

	HEADER(PQDequeue);
	TEST(SUCCESS == TestDequeue(queue), PQDequeue);

	PQEnqueue(queue, &num5);
	PQEnqueue(queue, &num6);
	PQEnqueue(queue, &num7);
	PQEnqueue(queue, &num8);
	
	HEADER(PQErase);
	TEST(num4 == *(int *)PQErase(queue, IsMatch, &num4), PQErase);
	TEST(num8 == *(int *)PQErase(queue, IsMatch, &num8), PQErase);
	TEST(NULL == PQErase(queue, IsMatch, &num8), PQErase - fail);

	HEADER(PQClear);
	PQClear(queue);
	TEST(0 == PQSize(queue), PQClear);

	PQDestroy(queue);
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

	return !(deq_num > *(int *)PQPeek(pq));
}

/************************************USER FUNCTIONS*************************************************/

int CmpInts(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}

int IsMatch(const void *num1, const void *num2)
{
	return (*(int *)num1 == *(int *)num2);
}
