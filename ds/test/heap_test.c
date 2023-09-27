/********************************************
* Developer: Avri Kehat						*
* Reviewed by:								*
* Date: xx.xx.23							*
* Description: Test file for				*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h>
#include <math.h>

#include "d_vector.h"
#include "heap.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/
#define LEFT_CHILD(index) (2 * index + 1) 
#define RIGHT_CHILD(index) (2 * index + 2) 
struct heap 
{
	vector_t *vec;
    int (*cmp_func)(const void *, const void *);
};

/********************************FORWARD DECLERATIONS*******************************************/
static void TestHeap(void);

static void TestMaxHeap(void);

static void TestMinHeap(void);

static int MaxHeapCmp(const void *lhs, const void *rhs);

static int MinHeapCmp(const void *lhs, const void *rhs);

static int IsMatch(const void *lhs, const void *rhs);

void PrintHeapTree(heap_t *heap);

static int HasHeapProperty(heap_t *heap, size_t index, int (*cmp_func)(const void *, const void *));

int main(void)
{
	TestHeap();
	
	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestHeap(void)
{
	TestMaxHeap();
	TestMinHeap();
}

static void TestMaxHeap(void)
{
	heap_t *heap_max = HeapCreate(MaxHeapCmp);
	int arr[] = {13, 5, 7, 2, 6, 9, 15, 4, 0, -4, 24};
	size_t size = (sizeof(arr) / sizeof(arr[0]));
	size_t i = 0;
	int to_remove = 0;
	int remove_fail = 132;
	int success_flag = SUCCESS;

	HEADER(HeapCreate - Max);
	TEST(NULL != heap_max, HeapCreate);
	TEST(TRUE == HeapIsEmpty(heap_max), HeapIsEmpty);
	TEST(0 == HeapSize(heap_max), HeapSize);

	HEADER(HeapPush);
	for (i = 0; i < size && SUCCESS == success_flag; ++i)
	{
		success_flag = HeapPush(heap_max, arr + i);
	}
	TEST(SUCCESS == success_flag, HeapPush);
	TEST(TRUE == HasHeapProperty(heap_max, 0, MaxHeapCmp), Validate Heap);

	HEADER(HeapSize);
	TEST(size == HeapSize(heap_max), HeapSize);
	
	HEADER(HeapRemove);
	to_remove = arr[5];
	TEST(arr[5] == *(int *)HeapRemove(heap_max, IsMatch, &to_remove), HeapRemove);
	to_remove = arr[9];
	TEST(arr[9] == *(int *)HeapRemove(heap_max, IsMatch, &to_remove), HeapRemove);
	TEST(TRUE == HasHeapProperty(heap_max, 0, MaxHeapCmp), Validate Heap);
	TEST(NULL == HeapRemove(heap_max, IsMatch ,&remove_fail), HeapRemove - fail);
	
	HEADER(HeapRemoveByKey);
	to_remove = arr[3];
	TEST(arr[3] == *(int *)HeapRemoveByKey(heap_max, &to_remove), HeapRemoveByKey);
	to_remove = arr[1];
	TEST(arr[1] == *(int *)HeapRemoveByKey(heap_max, &to_remove), HeapRemoveByKey);
	TEST(TRUE == HasHeapProperty(heap_max, 0, MaxHeapCmp), Validate Heap);
	TEST(NULL == HeapRemoveByKey(heap_max, &remove_fail), RemoveByKey - fail);

	HEADER(HeapPeek);
	TEST(24 == *(int *)HeapPeek(heap_max), HeapPeek);

	HEADER(HeapPop);
	HeapPop(heap_max);
	HeapPop(heap_max);
	HeapPop(heap_max);
	TEST(TRUE == HasHeapProperty(heap_max, 0, MaxHeapCmp), Validate Heap);
	
	HEADER(HeapDestroy);
	HeapDestroy(heap_max);
}

static void TestMinHeap(void)
{
	heap_t *heap_min = HeapCreate(MinHeapCmp);
	int arr[] = {13, 5, 7, 2, 6, 9, 15, 4, 0, -4, 24};
	size_t size = (sizeof(arr) / sizeof(arr[0]));
	size_t i = 0;
	int to_remove = 0;
	int remove_fail = 132;
	int success_flag = SUCCESS;

	HEADER(HeapCreate - Min);
	TEST(NULL != heap_min, HeapCreate);
	TEST(TRUE == HeapIsEmpty(heap_min), HeapIsEmpty);
	TEST(0 == HeapSize(heap_min), HeapSize);

	HEADER(HeapPush);
	for (i = 0; i < size && SUCCESS == success_flag; ++i)
	{
		success_flag = HeapPush(heap_min, arr + i);
	}
	TEST(SUCCESS == success_flag, HeapPush);
	TEST(TRUE == HasHeapProperty(heap_min, 0, MinHeapCmp), Validate Heap);

	HEADER(HeapSize);
	TEST(size == HeapSize(heap_min), HeapSize);

	HEADER(HeapPeek);
	TEST(-4 == *(int *)HeapPeek(heap_min), HeapPeek);
	
	HEADER(HeapRemove);
	to_remove = arr[5];
	TEST(arr[5] == *(int *)HeapRemove(heap_min, IsMatch, &to_remove), HeapRemove);
	to_remove = arr[9];
	TEST(arr[9] == *(int *)HeapRemove(heap_min, IsMatch, &to_remove), HeapRemove);
	to_remove = arr[3];
	TEST(arr[3] == *(int *)HeapRemove(heap_min, IsMatch, &to_remove), HeapRemove);
	TEST(TRUE == HasHeapProperty(heap_min, 0, MinHeapCmp), Validate Heap);
	TEST(NULL == HeapRemove(heap_min, IsMatch, &remove_fail), HeapRemove - fail);

	HEADER(HeapRemoveByKey);
	to_remove = arr[10];
	TEST(arr[10] == *(int *)HeapRemoveByKey(heap_min, &to_remove), HeapRemoveByKey);
	to_remove = arr[1];
	TEST(arr[1] == *(int *)HeapRemoveByKey(heap_min, &to_remove), HeapRemoveByKey);
	TEST(TRUE == HasHeapProperty(heap_min, 0, MinHeapCmp), Validate Heap);
	TEST(NULL == HeapRemoveByKey(heap_min, &remove_fail), RemoveByKey - fail);
	
	HEADER(HeapPop);
	HeapPop(heap_min);
	HeapPop(heap_min);
	TEST(TRUE == HasHeapProperty(heap_min, 0, MinHeapCmp), Validate Heap);

	HEADER(HeapDestroy);
	HeapDestroy(heap_min);
}
/************************************ User Functions *******************************************/

static int MaxHeapCmp(const void *lhs, const void *rhs)
{
	return (*(int *)lhs - *(int *)rhs);
}

static int MinHeapCmp(const void *lhs, const void *rhs)
{
	return (*(int *)rhs - *(int *)lhs);
}

static int IsMatch(const void *lhs, const void *rhs)
{
	return (*(int *)lhs == *(int *)rhs);
}

/************************************ Aux Functions *******************************************/
void PrintHeapTree(heap_t *heap) 
{
    size_t i = 0, j = 0, k = 0;
    size_t height = ceil(log(HeapSize(heap))/log(2)) + 1;
    size_t curr_level_size = 1, curr_level = 1;
    int *curr_level_elem = NULL;

    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < curr_level_size; ++j)
        {
            for (k = 0; k < pow(2, height - curr_level + 1) - 1; ++k)
            {
                printf(" ");
            }

            if (j + pow(2, curr_level - 1) - 1 >= HeapSize(heap))
            {
                break;
            }
            curr_level_elem = *(void **)VectorGetAccessToElement(heap->vec, j + pow(2, curr_level - 1) - 1);
            printf("%d", *curr_level_elem);

            for (k = 0; k < pow(2, height - curr_level + 2) - 1; ++k)
            {
                printf(" ");
            }
        }

        printf("\n");

        if (j + pow(2, curr_level - 1) - 1 >= HeapSize(heap))
        {
            break;
        }

        curr_level_size *= 2;
        ++curr_level;
    }
}

static int HasHeapProperty(heap_t *heap, size_t index, int(*cmp_func)(const void*, const void*))
{
	size_t size = HeapSize(heap);

	if ((size <= 1) || (index > size)) return 1;

	if ((LEFT_CHILD(index) < size &&
		cmp_func(*(void**)VectorGetAccessToElement(heap->vec, LEFT_CHILD(index)), 
			*(void**)VectorGetAccessToElement(heap->vec, index)) > 0) || 
		(RIGHT_CHILD(index) < size &&
		cmp_func(*(void**)VectorGetAccessToElement(heap->vec, RIGHT_CHILD(index)), 
			*(void**)VectorGetAccessToElement(heap->vec, index)) > 0))
	{ 
		return 0;
	}

	return HasHeapProperty(heap, LEFT_CHILD(index), cmp_func) &&
			HasHeapProperty(heap, RIGHT_CHILD(index), cmp_func);
}