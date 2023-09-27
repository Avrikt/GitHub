/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 28.03.23							*
* Description: Test file for sort			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*rand(), qsort*/
#include <time.h> /*clock()*/

#include "utilities.h"
#include "sort.h"

/*********************************GLOBAL VARRIABLES*********************************************/

#define MAX_ELEMENTS (5000)

/********************************FORWARD DECLERATIONS*******************************************/

static void TestQSort(void);
static void TestBubbleSort(void);
static void TestInsertionSort(void);
static void TestSelectionSort(void);
static void TestCountingSort(void);
static void TestRadixSort(void);
static void TestBinarySearch(void);
static void TestMergeSort(void);
static void FillArray(int arr[], size_t size);
static void FillArray2(int arr[], size_t size);
static int TestSortedArray(int array[], size_t size);
static void TestBenchMark(void (*test_function)(void));
int CmpInts(const int num1, const int num2);
int CmpFunc(const void *num1, const void *num2);

int main(void)
{
	TestBenchMark(TestQSort);
	TestBenchMark(TestBubbleSort);
	TestBenchMark(TestInsertionSort);
	TestBenchMark(TestSelectionSort);
	TestBenchMark(TestCountingSort);
	TestBenchMark(TestRadixSort);
	TestBenchMark(TestMergeSort);
	TestBinarySearch();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestBubbleSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray(arr, MAX_ELEMENTS);

	HEADER(BubbleSort);

	BubbleSort(arr, MAX_ELEMENTS, CmpInts);

	TEST(0 == TestSortedArray(arr, MAX_ELEMENTS), BubbleSort);
}

static void TestInsertionSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray(arr, MAX_ELEMENTS);

	HEADER(InsertionSort);

	InsertionSort(arr, MAX_ELEMENTS, CmpInts);

	TEST(0 == TestSortedArray(arr, MAX_ELEMENTS), InsertionSort);
}

static void TestSelectionSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray(arr, MAX_ELEMENTS);

	HEADER(SelectionSort);

	SelectionSort(arr, MAX_ELEMENTS, CmpInts);
	TEST(0 == TestSortedArray(arr, MAX_ELEMENTS), SelectionSort);
}

static void TestCountingSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray2(arr, MAX_ELEMENTS);

	HEADER(CountingSort);

	if (1 == CountingSort(arr, MAX_ELEMENTS))
	{
		return;
	}

	TEST(0 == TestSortedArray(arr, MAX_ELEMENTS), CountingSort);
}

static void TestRadixSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray2(arr, MAX_ELEMENTS);

	HEADER(RadixSort);

	if (1 == RadixSort(arr, MAX_ELEMENTS))
	{
		return;
	}

	TEST(0 == TestSortedArray(arr, 8), RadixSort);
}

static void TestBinarySearch(void)
{
	int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 ,11, 12};
	size_t size = sizeof(arr) / sizeof(arr[0]);
	int num = 3;
	int num1 = 8;
	int num2 = 132;

	HEADER(BinarySearchIterative);
	TEST(num == BinarySearchIterative(arr, size, arr[num]), BinarySearchIterative);
	TEST(num1 == BinarySearchIterative(arr, size, arr[num1]), BinarySearchIterative);
	TEST(-1 == BinarySearchIterative(arr, size, num2), BinarySearchIterative);

	HEADER(BinarySearchRecursive);
	TEST(num == BinarySearchRecursive(arr, size, arr[num]), BinarySearchRecursive);
	TEST(num1 == BinarySearchRecursive(arr, size, arr[num1]), BinarySearchRecursive);
	TEST(-1 == BinarySearchRecursive(arr, size, num2), BinarySearchRecursive);
}

static void TestMergeSort(void)
{
	int arr[MAX_ELEMENTS] = {0};
	size_t size = sizeof(arr) / sizeof(arr[0]);

	FillArray2(arr, MAX_ELEMENTS);

	HEADER(MergeSort);
	TEST(SUCCESS == MergeSort(arr, size), MergeSort);
	TEST(0 == TestSortedArray(arr, size), MergeSort);
}

static void TestQSort(void)
{
	int arr[MAX_ELEMENTS] = {0};

	FillArray2(arr, MAX_ELEMENTS);

	HEADER(QSort);

	QSort(arr, MAX_ELEMENTS, sizeof(int), CmpFunc);
	TEST(0 == TestSortedArray(arr, MAX_ELEMENTS), QSort);
}

static int TestSortedArray(int array[], size_t size)
{
	size_t i = 0;
	int success_flag = 0;

	for (i = 0; i < size - 1; ++i)
	{
		if(array[i] > array[i + 1])
		{
			success_flag = 1;
			break;
		}
	}

	return success_flag;
}

static void FillArray(int arr[], size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; ++i)
	{
		arr[i] = rand();
	}
}

static void FillArray2(int arr[], size_t size)
{
	size_t i = 0;

	for (i = 0; i < size; ++i)
	{
		arr[i] = (rand() % 100);
	}
}

static void TestBenchMark(void (*test_function)(void))
{
	clock_t start_time, end_time;

	start_time = clock();

	test_function();

	end_time = clock();

	printf("Total time taken by CPU: %f\n", (double)(end_time - start_time)/ CLOCKS_PER_SEC);
}

/************************************USER FUNCTIONS**********************************************/

int CmpInts(const int num1, const int num2)
{
	return (num1 - num2);
}

int CmpFunc(const void *num1, const void *num2)
{
	return *(int *)num1 - *(int *)num2;
}
