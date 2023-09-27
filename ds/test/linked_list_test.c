/********************
Developer: Avri Kehat
Reviewed by: Oran Frieden
Date: 08.02.23
*********************/

#include <stdio.h> /*printf*/
#include "../include/linked_list.h"

#define GRN ("\x1B[0;32m")
#define REDB ("\x1B[1;31m")
#define RESET ("\x1B[0m")
#define TEST_RESULT(FLAG) (!FLAG) ? printf("\n%sSuccess\n\n%s", GRN, RESET) \
								 : (printf("\n%sFailure, line %d\n\n%s", REDB, __LINE__, RESET))

static void TestLinkedList(void);
static int TestInsertandRemove(sll_t *sll);
static int TestSLLCount(size_t x, sll_t *sll);
static int TestGetData(int x, iterator_t iter);
static int TestSLLEndIter(iterator_t iter, sll_t *sll);
static int TestRemove(sll_t *sll, iterator_t iter);
static int TestForEach(sll_t *sll, int to_add);
static int AddToValue(void *a, void *b);
static int int_match(void *num1, const void *num2);

int main(void)
{
	TestLinkedList();	
	return 0;
}


static void TestLinkedList(void)
{
	sll_t *new_SLL = NULL;
	int test_flag = 0;
	
	new_SLL = SLLCreate();
	
	test_flag = SLLIsEmpty(new_SLL);
	printf("IsEmpty test:");
	TEST_RESULT(!test_flag);
	
	test_flag = TestInsertandRemove(new_SLL);

	SLLDestroy(new_SLL);
	
	if(0 == test_flag)
	{
		printf("%sAll tests passed successfully%s\n\n", GRN, RESET);
	}
	else
	{
		printf("%sYou're a terrible programer!!%s\n\n", REDB, RESET);
	}
}

static int TestInsertandRemove(sll_t *sll)
{
	int test_flag = 0;
	int x = 5;
	int x2 = 6;
	int x3 = 7;
	int x4 = 8;
	int to_add = 10;
	iterator_t iter1 = NULL;
	iterator_t iter2 = NULL;
	
	iter1 = SLLInsertBefore(SLLBeginIter(sll), &x);
	SLLInsertBefore(SLLNextIter(iter1), &x2);
	test_flag = !(SLLIsEmpty(sll));
	
	printf("Insertbefore test:");
	TEST_RESULT(!test_flag);
	
	SLLInsertBefore(iter1, &x3);
	test_flag = TestSLLCount(3, sll);

	printf("SLLCount test:");
	TEST_RESULT(test_flag);
	
	iter2 = SLLNextIter(iter1);
	iter2 = SLLNextIter(iter2);

	test_flag = TestSLLEndIter(iter2,sll);
	printf("SLLEndIter test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestGetData(x3, iter1);

	printf("SLLGetData test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestGetData(x2, iter2);
	
	test_flag = SLLIsSameIter(iter1, iter2);
	printf("SLLIsSameIter test:");
	TEST_RESULT(test_flag);
	
	iter1 = SLLInsertBefore(iter2, &x4);
	
	test_flag = !SLLIsSameIter(iter1, iter2);		/*Test that not same*/
	printf("Test that not the same:");
	TEST_RESULT(test_flag);
	
	iter1 = SLLFind(SLLBeginIter(sll), SLLEndIter(sll), &int_match, &x2);
	
	test_flag = SLLIsSameIter(iter1, iter2);
	printf("SLLfind test:");
	TEST_RESULT(test_flag);
	
	
	TestForEach(sll, to_add);
	
	test_flag = TestRemove(sll, iter2);
	printf("SLLRemove test:");
	TEST_RESULT(test_flag);

	SLLRemove(iter2);
	test_flag = SLLIsEmpty(sll);						/*Test that not empty*/
	printf("Test that not empty:");
	TEST_RESULT(test_flag);
	
	SLLRemove(iter1);
	SLLRemove(SLLBeginIter(sll));
	test_flag = !SLLIsEmpty(sll);
	printf("IsEmpty test:");
	TEST_RESULT(test_flag);
	
	return test_flag;
}

static int TestSLLEndIter(iterator_t iter, sll_t *sll)
{
	return (SLLIsSameIter(iter, SLLEndIter(sll)));
}

static int TestSLLCount(size_t x, sll_t *sll)
{
	return !(x == SLLCount(sll));
}

static int TestGetData(int x, iterator_t iter)
{
	return !(x == *((int *)SLLGetData(iter)));
}

static int TestRemove(sll_t *sll, iterator_t iter)
{
	size_t count = 0;
	
	count = SLLCount(sll);
	
	SLLRemove(iter);
	
	return !(count - 1 == SLLCount(sll));
}

static int TestForEach(sll_t *sll, int to_add)
{
	int test_add = 0;
	int test_flag = 0;
	iterator_t test_iter = SLLBeginIter(sll);
	
	test_add = *((int *)SLLGetData(test_iter));
	
	test_flag = SLLForEach(test_iter, SLLEndIter(sll), &AddToValue, &to_add);
	printf("ForEach test:");
	TEST_RESULT(test_flag);
	
	test_flag = (to_add == test_add - (*((int *)SLLGetData(test_iter))));
	return test_flag;
}

/******************User Functions***************/
static int AddToValue(void *a, void *b)
{
	*(int *)a += *(int *)b;
	return 0;
}

static int int_match(void *num1,const void *num2)
{
	return (*(int *)num1 == *(int *)num2);
}
