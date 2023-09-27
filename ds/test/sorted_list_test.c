/******************************
Developer: Avri Kehat
Reviewed by: Lilach Rabinovich
Date: 19.02.2023
*******************************/

#include <stdio.h> /*printf*/

#include "sorted_list.h"

#define GRN ("\x1B[0;32m")
#define REDB ("\x1B[1;31m")
#define RESET ("\x1B[0m")
#define TEST_RESULT(FLAG) (!FLAG) ? printf("\n%sSuccess\n\n%s", GRN, RESET) \
								 : (printf("\n%sFailure, line %d\n\n%s", REDB, __LINE__, RESET))

static void TestSortedList(void);
static int TestSortedListIsEmpty(const sol_t *sol);
static int TestSortedListIsSameIter(const sol_iterator_t iter1, const sol_iterator_t iter2);
static int TestSortedListSize(const sol_t *sol, size_t x);
static int TestGetData(sol_iterator_t iter, int x);
static int TestPop(sol_t *sol);
static int TestFindIf(sol_t *sol, sol_iterator_t iter, int num);
void PrintEach(sol_iterator_t from, const sol_iterator_t to);
int CmpInts(const void *num1, const void *num2);
int int_match(const void *num1, const void *num2);

/*********************test functions************************/

int main(void)
{
	TestSortedList();

	return 0;
}

static void TestSortedList(void)
{
	sol_t *first_sol = SortedListCreate(&CmpInts);
	sol_t *second_sol = SortedListCreate(&CmpInts);
	int test_flag = 0;
	int num1 = 7;
	int num2 = 1;
	int num3 = 5;
	int num4 = 16;
	int num5 = 10;
	int merge1 = 2;
	int merge2 = 4;
	int merge3 = 13;
	int merge4 = 3;
	sol_iterator_t iter1;

	test_flag = TestSortedListIsEmpty(first_sol);
	printf("IsEmpty test:");
	TEST_RESULT(test_flag);

	iter1 = SortedListInsert(first_sol, &num1);
	test_flag = TestSortedListIsSameIter(iter1, SortedListBeginIter(first_sol));
	printf("Insert test:");
	TEST_RESULT(test_flag);

	iter1 = SortedListInsert(first_sol, &num2);
	test_flag = TestSortedListIsSameIter(iter1, SortedListBeginIter(first_sol));
	printf("Insert second sorted item test:");
	TEST_RESULT(test_flag);

	SortedListInsert(first_sol, &num3);

	test_flag = TestGetData(SortedListBeginIter(first_sol), num2);
	printf("SortedListGetData test:");
	TEST_RESULT(test_flag);

	SortedListInsert(first_sol, &num4);
	SortedListInsert(first_sol, &num5);

	test_flag = TestSortedListSize(first_sol, 5);
	printf("SortedListSize test:");
	TEST_RESULT(test_flag);

	test_flag = TestPop(first_sol);
	printf("All Pop tests:");
	TEST_RESULT(test_flag);

	iter1 = SortedListInsert(first_sol, &num4);
	SortedListInsert(first_sol, &num5);
	SortedListInsert(first_sol, &num2);

	test_flag = TestFindIf(first_sol, iter1, num4);
	printf("FindIf test:");
	TEST_RESULT(test_flag);

	SortedListInsert(second_sol, &merge1);
	SortedListInsert(second_sol, &merge2);
	SortedListInsert(second_sol, &merge3);
	SortedListInsert(second_sol, &merge4);

	printf("First list before merge\n");
	PrintEach(SortedListBeginIter(first_sol), SortedListEndIter(first_sol));

	printf("\nSecond list before merge\n");
	PrintEach(SortedListBeginIter(second_sol), SortedListEndIter(second_sol));

	SortedListMerge(first_sol, second_sol);

	printf("\nFirst list after merge\n");
	PrintEach(SortedListBeginIter(first_sol), SortedListEndIter(first_sol));

	SortedListDestroy(first_sol);
	SortedListDestroy(second_sol);

	if (0 == test_flag)
	{
		printf("%sAll tests passed successfully%s\n\n", GRN, RESET);
	}
	else
	{
		printf("%sYou're a terrible programer!!%s\n\n", REDB, RESET);
	}
}

static int TestSortedListIsEmpty(const sol_t *sol)
{
	return !SortedListIsEmpty(sol);
}

static int TestSortedListIsSameIter(const sol_iterator_t iter1, const sol_iterator_t iter2)
{
	return !SortedListIsSameIter(iter1, iter2);
}

static int TestSortedListSize(const sol_t *sol, size_t x)
{
	return !(x == SortedListSize(sol));
}

static int TestGetData(sol_iterator_t iter, int x)
{
	return !(x == *((int *)SortedListGetData(iter)));
}

static int TestFindIf(sol_t *sol, sol_iterator_t iter, int num)
{
	sol_iterator_t test = {0};
	test = SortedListFindIf(SortedListBeginIter(sol), SortedListEndIter(sol), &int_match, &num);

	return !SortedListIsSameIter(iter, test);
}
static int TestPop(sol_t *sol)
{
	int test_flag = 0;
	int test_value = 0;

	test_value = *(int *)SortedListGetData(SortedListBeginIter(sol));

	test_flag += !(test_value == *(int *)SortedListPopFront(sol));
	printf("PopFront test:");
	TEST_RESULT(test_flag);

	test_value = *(int *)SortedListGetData(SortedListPrevIter(SortedListEndIter(sol)));

	test_flag += !(test_value == *(int *)SortedListPopBack(sol));
	printf("PopBack test:");
	TEST_RESULT(test_flag);

	return test_flag;
}

/******************User Functions***************/

int CmpInts(const void *num1, const void *num2)
{
	return (*(int *)num1 - *(int *)num2);
}

int int_match(const void *num1, const void *num2)
{
	return (*(int *)num1 == *(int *)num2);
}

void PrintEach(sol_iterator_t from, const sol_iterator_t to)
{
	sol_iterator_t runner = from;
	int i = 1;

	while (0 == SortedListIsSameIter(to, runner))
	{
		printf("The value of the %d elment is %d \n",i , *(int *)SortedListGetData(runner));
		runner = SortedListNextIter(runner);
		++i;
	}
}
