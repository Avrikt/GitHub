/********************
Developer: Avri Kehat
Reviewed by: Noam Gazit
Date: 16.02.2023
Description: Implementation of Doubly Linked List
*********************/

#include <stdio.h> /*printf*/
#include "dll.h"

#define GRN ("\x1B[0;32m")
#define REDB ("\x1B[1;31m")
#define RESET ("\x1B[0m")
#define TEST_RESULT(FLAG) (!FLAG) ? printf("\n%sSuccess\n\n%s", GRN, RESET) \
								 : (printf("\n%sFailure, line %d\n\n%s", REDB, __LINE__, RESET))

static void TestDLinkedList(void);
static int TestDLLIsEmpty(const dll_t *dll);
static int TestDLLIsSameIter(const iterator_t iter1, const iterator_t iter2);
static int TestInsertandRemove(dll_t *dll);
static int TestDLLCount(const dll_t *dll, size_t x);
static int TestGetData(iterator_t iter, int x);
static int TestSetData(iterator_t iter, int x);
static int TestPushFront(const iterator_t iter1, const iterator_t iter2);
static int TestPushBack(const iterator_t iter1, const iterator_t iter2);
static int TestPop(dll_t *dll);
static int TestSplice(dll_t *dll1, dll_t *dll2);
static int int_match(const void *num1, const void *num2);
void TestMulti(dll_t *dll_dest, iterator_t from, iterator_t to, is_match_t user_func, void *param);

int main(void)
{
	TestDLinkedList();
	return 0;
}

static void TestDLinkedList(void)
{
	dll_t *new_DLL = DLLCreate();
	dll_t *second_DLL = DLLCreate();
	int test_flag = 0;
	int num1 = 5;
	int x = 3;
	int x2 = 13;
	int x3 = 33;
	
	test_flag = TestDLLIsEmpty(new_DLL);
	printf("IsEmpty test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestInsertandRemove(new_DLL);
	
	DLLPushFront(new_DLL, &num1);
	DLLPushFront(new_DLL, &x);
	DLLPushFront(new_DLL, &x3);
	DLLPushFront(new_DLL, &x2);
	DLLPushFront(new_DLL, &x3);
	
	TestMulti(second_DLL, DLLBeginIter(new_DLL), DLLEndIter(new_DLL), &int_match, &x2);
	
	DLLPushFront(second_DLL, &x);
	DLLPushFront(second_DLL, &x2);
	
	test_flag = TestSplice(new_DLL, second_DLL);
	printf("Splice test:");
	TEST_RESULT(test_flag);
	
	DLLDestroy(new_DLL);
	DLLDestroy(second_DLL);
	if(0 == test_flag)
	{
		printf("%sAll tests passed successfully%s\n\n", GRN, RESET);
	}
	else
	{
		printf("%sYou're a terrible programer!!%s\n\n", REDB, RESET);
	}
}

static int TestInsertandRemove(dll_t *dll)
{
	int test_flag = 0;
	int x = 5;
	int x2 = 2;
	int x3 = 3;
	int x4 = 4;
	iterator_t iter1 = NULL;
	iterator_t iter2 = NULL;
	
	iter1 = DLLInsert(DLLBeginIter(dll), &x);
	test_flag = TestDLLIsSameIter(iter1, DLLBeginIter(dll));
	printf("Insert test:");
	TEST_RESULT(test_flag);
	
	iter2 = DLLInsert(iter1, &x2);
		
	test_flag = TestGetData(DLLBeginIter(dll), x2);
	printf("DLLGetData test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestDLLCount(dll, 2);
	printf("DLLCount after 2 inserts test:");
	TEST_RESULT(test_flag);
	
	DLLRemove(iter2);
	test_flag = TestDLLCount(dll, 1);
	printf("DLLCount after 1 remove test:");
	TEST_RESULT(test_flag);
	
	iter1 = DLLPushBack(dll, &x3);
	test_flag = TestPushBack(DLLPrevIter(DLLEndIter(dll)), iter1);
	printf("PushBack test:");
	TEST_RESULT(test_flag);
	
	iter1 = DLLPushFront(dll, &x4);
	test_flag = TestPushFront(DLLBeginIter(dll), iter1);
	printf("PushFront test:");
	TEST_RESULT(test_flag);
	
	DLLSetData(iter1, &x3);
	test_flag = TestSetData(iter1, x3);
	printf("DLLSetData test:");
	TEST_RESULT(test_flag);
	
	DLLPushFront(dll, &x);
	test_flag = TestDLLCount(dll, 4);
	printf("DLLCount after 3 pushes test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestPop(dll);
	printf("All Pop tests:");
	TEST_RESULT(test_flag);
	
	DLLPopFront(dll);
	DLLPopBack(dll);
	
	test_flag = TestDLLIsEmpty(dll);
	printf("IsEmpty test:");
	TEST_RESULT(test_flag);
	
	return test_flag;
}

static int TestDLLIsEmpty(const dll_t *dll)
{
	return !DLLIsEmpty(dll);
}

static int TestDLLIsSameIter(const iterator_t iter1, const iterator_t iter2)
{
	return !DLLIsSameIter(iter1, iter2);
}

static int TestDLLCount(const dll_t *dll, size_t x)
{
	return !(x == DLLCount(dll));
}

static int TestGetData(iterator_t iter, int x)
{
	return !(x == *((int *)DLLGetData(iter)));
}

static int TestSetData(iterator_t iter, int x)
{
	return !(x == *((int *)DLLGetData(iter)));
}

static int TestPushFront(const iterator_t iter1, const iterator_t iter2)
{
	return TestDLLIsSameIter(iter1, iter2);
}

static int TestPushBack(const iterator_t iter1, const iterator_t iter2)
{
	return TestDLLIsSameIter(iter1, iter2);
}

static int TestPop(dll_t *dll)
{
	int test_flag = 0;
	int test_value = 0;
	
	test_value = *(int *)DLLGetData(DLLBeginIter(dll));
	
	test_flag += !(test_value == *(int *)DLLPopFront(dll));
	printf("PopFront test:");
	TEST_RESULT(test_flag);
	
	test_value = *(int *)DLLGetData(DLLPrevIter(DLLEndIter(dll)));
	
	test_flag += !(test_value == *(int *)DLLPopBack(dll));
	printf("PopBack test:");
	TEST_RESULT(test_flag);
	
	return test_flag; 
}

static int TestSplice(dll_t *dll1, dll_t *dll2)
{
	size_t dll1_size = DLLCount(dll1);
	size_t dll2_size = DLLCount(dll2);
	
	DLLSplice(DLLBeginIter(dll1), DLLBeginIter(dll2), DLLEndIter(dll2));
		
	return !(dll1_size + dll2_size == DLLCount(dll1));
}	

void TestMulti(dll_t *dll_dest, iterator_t from, iterator_t to, is_match_t user_func, void *param)
{
	iterator_t iter = from;
	
	printf("Values of first list:\n"); 
	while (iter != to)
	{
		printf("%d ", *(int *)DLLGetData(iter));	
		iter = DLLNextIter(iter);
	}
	printf("\n\n");
	
	dll_dest = DLLMultiFind(dll_dest, from, to, user_func, param);
	
	iter = DLLBeginIter(dll_dest);
	
	printf("The MultiFind function finds all nodes with value of %d or bigger: \n", *(int *)param);
	
	while (!DLLIsSameIter(iter, DLLEndIter(dll_dest)))
	{
		printf("%d ", *(int *)DLLGetData(iter));	
		iter = DLLNextIter(iter);
	}
	printf("\n\n");
}


/******************User Functions***************/

static int int_match(const void *num1, const void *num2)
{
	return (*(int *)num1 == *(int *)num2);
}

