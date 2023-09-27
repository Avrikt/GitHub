/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 12.03.23							*
* Description: Test file for VSA			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc, free*/

#include "vsa.h"

/************************************TEST MACRO*************************************************/
#define TEST(condition, func_name) ((1) == (condition))?\
									printf("%-25s:\x1b[32m Success \x1b[0m\n", #func_name) : \
								 	printf("%-25s:\x1b[1;31m Failure, Line: %d \x1b[0m\n",\
								 	#func_name, __LINE__)

#define PRINTRESULT(x) ((0) == (x)) ?\
printf("\t\x1b[32m %ld \x1b[0m | Line: %d\n", x , __LINE__) : \
printf("\t\x1b[31m %ld \x1b[0m | Line: %d\n", x , __LINE__)

/************************************PROTOTYPES**************************************************/
static void TestVSA(void);
static int TestAlloc(vsa_t *vsa, void *address, size_t offset);
static int TestDefragment(vsa_t *vsa, void *alloc1, void *alloc2);

int main(void)
{
	TestVSA();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestVSA(void)
{
	vsa_t *new_vsa = NULL;
	void *memory = malloc(160);
	void *alloc = NULL;
	void *alloc2 = NULL;
	void *alloc_fail = NULL;

	new_vsa = VSAInit(memory, 160);

	TEST(new_vsa == memory, VSAInit);

	TEST(144 == VSALargestChunkAvailable(new_vsa), LargestChunkAvailable);

	alloc = VSAAlloc(new_vsa, 5);
	TEST(0 == TestAlloc(new_vsa, alloc, 8), VSAalloc);
	TEST(128 == VSALargestChunkAvailable(new_vsa), LargestChunkAvailable);

	alloc = VSAAlloc(new_vsa, 37);
	/*Test offset of 24 -> 8 for header, 16 for previous block*/
	TEST(0 == TestAlloc(new_vsa, alloc, 24), VSAAlloc);

	alloc2 = VSAAlloc(new_vsa, 69);
	TEST((NULL != alloc2), VSAAlloc);
	TEST(0 == VSALargestChunkAvailable(new_vsa), LargestChunkAvailable);

	alloc_fail = VSAAlloc(new_vsa, 8);
	TEST((NULL == alloc_fail), VSAAllocFail);

	TEST(0 == TestDefragment(new_vsa, alloc2, alloc), TestDefragment);
	free(memory);
}

static int TestAlloc(vsa_t *vsa, void *address, size_t offset)
{
	return !((offset + 8) == (size_t)((char *)address - (char *)vsa));
}

static int TestDefragment(vsa_t *vsa, void *alloc1, void *alloc2)
{
	int success_flag = 0;

	VSAFree(alloc1);

	success_flag += !(80 == VSALargestChunkAvailable(vsa));

	alloc1 = VSAAlloc(vsa, 128);
	success_flag += !(NULL == alloc1);

	VSAFree(alloc2);
	success_flag += !(128 == VSALargestChunkAvailable(vsa));

	return success_flag;
}
