/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 07.03.23							*
* Description: Test file for FSA			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h>
#include <stdlib.h> /*malloc, free*/

#include "fsa.h"

/************************************TEST MACRO*************************************************/
#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-20s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-20s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) ((0) == (x)) ?\
printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)

/************************************PROTOTYPES**************************************************/

static void TestFSA(void);
static int TestOverFlow(fsa_t *fsa);

int main(void)
{
	TestFSA();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestFSA(void)
{
	fsa_t *new_fsa = NULL;
	void *test_memory =  malloc(168);
	void* alloc1 = NULL;
	void* alloc2 = NULL;

	TEST (168 == FSASuggestSize(10,10), FSASuggestSize);
	TEST (72 == FSASuggestSize(8,8), FSASuggestSize);
	TEST (72 == FSASuggestSize(8,7), FSASuggestSize);
	TEST (136 == FSASuggestSize(8,16), FSASuggestSize);

	new_fsa = FSAInit(test_memory, 168, 10);

	TEST(test_memory == new_fsa, FSAInit);

	TEST(10 == FSACountFree(new_fsa), FSACountFree);

	alloc1 = FSAAlloc(new_fsa);
	TEST(NULL != alloc1, FSAAlloc);

	TEST(9 == FSACountFree(new_fsa), FSACountFree);

	alloc2 = FSAAlloc(new_fsa);
	TEST(NULL != alloc1, FSAAlloc);

	FSAFree(new_fsa, alloc1);
	FSAFree(new_fsa, alloc2);

	TEST(10 == FSACountFree(new_fsa), FSAFree);

	free(test_memory);

	test_memory = malloc(16);
	new_fsa = FSAInit(test_memory, 16, 8);

	TEST(0 == TestOverFlow(new_fsa), TestAllocOverFlow);

	free(test_memory);
}

static int TestOverFlow(fsa_t *fsa)
{
	void *alloc = NULL;

	alloc = FSAAlloc(fsa);
	alloc = FSAAlloc(fsa);

	return !(NULL == alloc);
}
