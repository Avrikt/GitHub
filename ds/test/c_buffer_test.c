/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 15.02.23
Description: Implementation of C_buffer
*********************/

#include <stdio.h> /*printf, size_t*/
#include "c_buffer.h"

#define GRN ("\x1B[0;32m")
#define REDB ("\x1B[1;31m")
#define RESET ("\x1B[0m")
#define TEST_RESULT(FLAG) (0 == FLAG) ? printf("\n%sSuccess\n\n%s", GRN, RESET) \
								 : (printf("\n%sFailure, line %d\n\n%s", REDB, __LINE__, RESET))
	
static void TestCBuffer(void);
static int TestBufferIsEmpty(c_buffer_t *buffer);
static int TestCBufferSize(c_buffer_t *buffer, size_t size);
static int TestCBufferFreeSpace(c_buffer_t *buffer, size_t size);
static int TestCBufferWrite(c_buffer_t *buffer, const char *str, size_t size);
static int TestCBufferRead(c_buffer_t *buffer, char *str, size_t size);
							 
int main(void)
{
	TestCBuffer();
	
	return 0;
}

static void TestCBuffer(void)
{
	int test_flag = 0;
	size_t buffer_size = 8;
	char str[10] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'H'};
	char str2[10] = {0};
	
	c_buffer_t *new_buffer = CBufferCreate(buffer_size);
	
	test_flag = TestBufferIsEmpty(new_buffer);
	printf("BufferIsEmpty test:");
	TEST_RESULT(test_flag);
		
	test_flag = TestCBufferSize(new_buffer, 0);
	printf("BufferSize test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestCBufferFreeSpace(new_buffer, buffer_size);
	printf("CBufferFreeSpace test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestCBufferWrite(new_buffer, str, 5);
	printf("CBufferWrite test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestCBufferSize(new_buffer, 5);
	printf("BufferSize test:");
	TEST_RESULT(test_flag);
	
	/*Test overflow
	test_flag = TestCBufferWrite(new_buffer, str, 1);
	printf("CBufferWrite test:");
	TEST_RESULT(test_flag);
	*/
	
	test_flag = TestCBufferRead(new_buffer, str2, 5);
	printf("CBufferRead test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestCBufferWrite(new_buffer, str, 4);
	printf("CBufferWrite test:");
	TEST_RESULT(test_flag);
	
	test_flag = TestCBufferRead(new_buffer, str2, 2);
	printf("CBufferRead test:");
	TEST_RESULT(test_flag);													
															
	/* Test underflow
	test_flag += TestCBufferRead(new_buffer, str2, 5);
	printf("CBufferRead test:");
	TEST_RESULT(test_flag);
	*/
	CBufferDestroy(new_buffer);
}


static int TestBufferIsEmpty(c_buffer_t *buffer)
{
	return !(CBufferIsEmpty(buffer));
}

static int TestCBufferSize(c_buffer_t *buffer, size_t size)
{
	return !(size == CBufferSize(buffer));
}

static int TestCBufferFreeSpace(c_buffer_t *buffer, size_t size)
{
	return !(size == CBufferFreeSpace(buffer));
}

static int TestCBufferWrite(c_buffer_t *buffer, const char *str, size_t size)
{
	ssize_t test_flag = 0;
	test_flag = CBufferWrite(buffer, str, size);
	
	if(test_flag != (ssize_t)size)
	{
		perror("Writing Error");
	}
	return !(test_flag == (ssize_t)size);
}

static int TestCBufferRead(c_buffer_t *buffer, char *str, size_t size)
{
	ssize_t test_flag = 0;
	test_flag = CBufferRead(buffer, str, size);
	
	if(test_flag != (ssize_t)size)
	{
		perror("Reading Error");
	}
	return !(test_flag == (ssize_t)size);
}
