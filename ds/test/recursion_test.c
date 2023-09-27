/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Mor Zibel					*
* Date: 15.03.23							*
* Description: Test file for Recursion		*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h> /*printf*/
#include <string.h> /*strlen, strcmp, strstr*/

#include "utilities.h"
#include "recursion.h"

/*********************************GLOBAL VARRIABLES*********************************************/

#define BUFFER (100)

/********************************FORWARD DECLERATIONS*******************************************/
static void TestRecursion(void);
static void TestStrLen(void);
static void TestStrCmp(void);
static void TestStrCpy(void);
static void TestStrCat(void);
static void TestStrStr(void);
static void TestSLLFLip(void);
static void TestSortStack(void);
static int IsStackSorted(stack_t *stack);

int main(void)
{
	TestRecursion();
	TestStrLen();
	TestStrCmp();
	TestStrCpy();
	TestStrCat();
	TestStrStr();
	TestSLLFLip();
	TestSortStack();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestRecursion(void)
{
	HEADER(Fibonacci iteration);
	TEST(5 == FibonacciIter(5), FibonacciIter);
	TEST(13 == FibonacciIter(7), FibonacciIter);
	TEST(4181 == FibonacciIter(19), FibonacciIter);

	HEADER(Fibonacci recursion);
	TEST(5 == FibonacciRec(5), FibonacciRec);
	TEST(13 == FibonacciRec(7), FibonacciRec);
	TEST(4181 == FibonacciRec(19), FibonacciRec);
}

static void TestStrLen(void)
{
	char *str = "hello";
	char *str2 = "Somebody told me";
	char *str3 = "I'm going to stand here like a unicorn";

	HEADER(StrLen Recursion);

	TEST((strlen(str) == StrLenRec(str)), StrLenRec);
	TEST((strlen(str2) == StrLenRec(str2)), StrLenRec);
	TEST((strlen(str3) == StrLenRec(str3)), StrLenRec);
}

static void TestStrCmp(void)
{
	char *s1= "abcd";
	char *s2= "abcg";
	char *s3= "bcea";
	char *s4= "abcde";
	char *s5= "";

	HEADER(StrCmp Recursion);
	TEST(strcmp(s1, s2) == StrCmpRec(s1, s2), StrCmpRec);
	TEST(strcmp(s1, s3) == StrCmpRec(s1, s3), StrCmpRec);
	TEST(strcmp(s1, s4) == StrCmpRec(s1, s4), StrCmpRec);
	TEST(strcmp(s2, s3) == StrCmpRec(s2, s3), StrCmpRec);
	TEST(strcmp(s4, s3) == StrCmpRec(s4, s3), StrCmpRec);
	TEST(strcmp(s4, s5) == StrCmpRec(s4, s5), StrCmpRec);

}

static void TestStrCpy(void)
{
	char *s1 = "copy this";
	char *s2 = "This can also be copied";
	char *s3 = "Trilili Tralala";
	char dest[BUFFER] = {0};

	HEADER(StrCpy Recursion);

	StrCpyRec(dest, s1);
	TEST(0 == strcmp(dest, s1), StrCpyRec);

	StrCpyRec(dest, s2);
	TEST(0 == strcmp(dest, s2), StrCpyRec);

	StrCpyRec(dest, s3);
	TEST(0 == strcmp(dest, s3), StrCpyRec);
}

static void TestStrCat(void)
{
	char dest[BUFFER] = "This is dest.";
	char *s1 = " copy this.";
	char *s2 = " This can also be copied.";
	char *to_check = "This is dest. copy this. This can also be copied.";

	HEADER(StrCat Recursion);

	StrCatRec(dest, s1);
	StrCatRec(dest, s2);
	TEST(0 == strcmp(dest, to_check), StrCatRec);
}

static void TestStrStr(void)
{
	char *haystack = "string";
	char *needle = "ring";
	char *needle2 = "str";
	char *fail = "This will fail";

	HEADER(StrStr Recursion);

	TEST(0 == strcmp(strstr(haystack, needle), StrStrRec(haystack, needle)), StrStrRec);
	TEST(NULL == StrStrRec(haystack, fail), StrStrRec);
	TEST(0 == strcmp(strstr(haystack, needle2), StrStrRec(haystack, needle2)), StrStrRec);
}

static void TestSLLFLip(void)
{
	int result = 1;
	node_t head;
    node_t two;
    node_t three;
    node_t four;
    node_t five;
    node_t tail;

    head.next = &two;
    two.next = &three;
    three.next = &four;
    four.next = &five;
    five.next = &tail;
    tail.next = NULL;

	HEADER(SLLFlip Recursion);
	FlipList(&head);

	result &= (tail.next == &five);
	result &= (five.next == &four);
	result &= (four.next == &three);
	result &= (three.next == &two);
	result &= (two.next == &head);
	result &= (head.next == NULL);

	TEST(1 == result, SLLFlip);
}

static void TestSortStack(void)
{
	stack_t *stack = StackCreate(10, sizeof(int));
	int num1 = 2;
	int num2 = 5;
	int num3 = -12;
	int num4 = 4;
	int num5 = 32;
	int num6 = 14;
	int num7 = 0;
	int num8 = -3;
	int num9 = 77;
	int num10 = 8;

	HEADER(SortStack Recursive);

	StackPush(stack, &num1);
	StackPush(stack, &num2);
	StackPush(stack, &num3);
	StackPush(stack, &num4);
	StackPush(stack, &num5);
	StackPush(stack, &num6);
	StackPush(stack, &num7);
	StackPush(stack, &num8);
	StackPush(stack, &num9);
	StackPush(stack, &num10);

	SortStack(stack);

	TEST(1 == IsStackSorted(stack), SortStack);

	StackDestroy(stack);
}

static int IsStackSorted(stack_t *stack)
{
	int success_flag = 1;
	int test_value = 0;

	while(1 < StackSize(stack))
	{
		test_value = *(int *)StackPeek(stack);
		StackPop(stack);
		success_flag &= (test_value > *(int *)StackPeek(stack));
	}

	return success_flag;
}
