/********************
Developer: Avri Kehat
Reviewed by: Omri Shalev
Date: 03.02.23
*********************/

#include <stdio.h> /*printf*/
#include "../include/d_vector.h"

static int TestIntDynamicVector(void);
static int TestCharDynamicVector(void);

int main(void)
{
	int success = 0;
	
	success = TestIntDynamicVector();
	success += TestCharDynamicVector();
	if(0 == success)
	{
		printf("All tests past successfully\n");
	}
	return 0;
}

/*dynamic vector of ints*/
static int TestIntDynamicVector(void)
{
	int a =1, b = 5;
	int i = 0;
	int result_of_tests = 0;
	
	vector_t *vector = VectorCreate((size_t)4, sizeof(int));
	
	
	printf("Vector size: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	for(i = 0; i < 5; ++i)
	{
		result_of_tests += VectorPushBack(vector, (const void *)&a);
		result_of_tests += VectorPushBack(vector, (const void *)&b);
	}
	printf("Size of vector after pushing 10 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	for(i = 0; i < 6; ++i)
	{
		VectorPopBack(vector);
	}
	
	printf("Size of vector after PopBack of 6 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	for(i = 0; i < 4; ++i)
	{
		VectorPopBack(vector);
	}
	printf("Size of vector after PopBack of 4 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	if(1 == VectorIsEmpty(vector))
	{
		printf("Vector is empty\n");
	}
	else
	{
		printf("Vector isn't empty\n");
	}
	
	VectorReserve(vector, 16);
	printf("Vector Capacity after reserve 16: %ld\n", VectorCapacity(vector));
	
	result_of_tests += VectorPushBack(vector, (const void *)&a);
	result_of_tests += VectorPushBack(vector, (const void *)&b);
	
	printf("Size of vector after Push of 2 items: %ld\n", VectorSize(vector));
	result_of_tests += VectorShrinkToSize(vector);
	
	printf("Vector Capacity after Shrink: %ld\n", VectorCapacity(vector));
	
	result_of_tests += VectorPushBack(vector, (const void *)&a);	
	printf("One more item pushed\n");
	
	printf("Vector Capacity after one push: %ld\n", VectorCapacity(vector));
	
	printf("Item at index 1 should be 5: %d\n", *(int *)VectorGetAccessToElement(vector, 1));
	
	VectorDestroy(vector);
	printf("Vector destroyed!\n\n");
	printf("*************************************************************\n\n");
	return 0 + result_of_tests;
}

static int TestCharDynamicVector(void)
{
	/*dynamic vector of chars*/
	char ch1 = 'c', ch2 = 'd'; 
	int i = 0;
	vector_t *vector = VectorCreate((size_t)12, sizeof(char));
	
	printf("Vector size: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	for(i = 0; i < 5; ++i)
	{
		VectorPushBack(vector, (const void *)&ch1);
		VectorPushBack(vector, (const void *)&ch2);
	}
	printf("Size of vector after pushing 10 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	for(i = 0; i < 7; ++i)
	{
		VectorPopBack(vector);
	}
	
	printf("Size of vector after PopBack of 7 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	for(i = 0; i < 2; ++i)
	{
		VectorPopBack(vector);
	}
	printf("Size of vector after PopBack of 2 items: %ld\n", VectorSize(vector));
	printf("Vector Capacity: %ld\n", VectorCapacity(vector));
	
	if(1 == VectorIsEmpty(vector))
	{
		printf("Vector is empty\n");
	}
	else
	{
		printf("Vector isn't empty\n");
	}
	
	VectorReserve(vector, 16);
	printf("Vector Capacity after reserve 16: %ld\n", VectorCapacity(vector));
	
	VectorPushBack(vector, (const void *)&ch1);
	VectorPushBack(vector, (const void *)&ch2);
	
	printf("Size of vector after Push of 2 items: %ld\n", VectorSize(vector));
	VectorShrinkToSize(vector);
	
	printf("Vector Capacity after Shrink: %ld\n", VectorCapacity(vector));
	
	VectorPushBack(vector, (const void *)&ch1);	
	printf("One more item pushed\n");
	
	printf("Vector Capacity after one push: %ld\n", VectorCapacity(vector));
	
	printf("Item at index 2 should be 'd': '%c'\n", *(char *)VectorGetAccessToElement(vector, 2));
	/*********Test for overflow*********
	printf("Item at index 6: '%c'\n", *(char *)VectorGetAccessToElement(vector, 6));
	printf("Item at index 10: %d\n", *(char *)VectorGetAccessToElement(vector, 10));*/
	
	VectorDestroy(vector);		
	printf("Vector destroyed!\n\n");
	return 0;
	
}
