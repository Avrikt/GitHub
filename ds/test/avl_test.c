/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Dave Hairapetian				*
* Date: 23.03.23							*
* Description: Test file for AVL			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <stdio.h>

#include "avl.h"
#include "utilities.h"

/*********************************GLOBAL VARRIABLES*********************************************/
#define RESULT_STATUS(status, expected_status, func_name) if((status) == (expected_status)) \
									{	printf("%-30s\t \033[0;32m SUCCESS\n\033[0m", #func_name);	 }\
									else {	printf("%-30s\t \033[0;31m FAILURE | Line: %d\n\033[0m", #func_name, __LINE__); }


typedef enum child_pos
{
	LEFT = 0,
	RIGHT = 1,
	NUM_OF_CHILDREN = 2
}child_pos_t;

typedef struct avl_node avl_node_t;

struct avl_node
{
	avl_node_t *children[NUM_OF_CHILDREN];
	void *data;
	size_t height;
} ;

struct avl_tree
{
	avl_node_t dummy_root;
	int (*cmp_func)(const void *, const void *);
};

static void TestAVL(void);
static int TestFind(avl_t *avl, int *value);
static void PrintTree(avl_node_t *node, int level);
static int VerifyTree(avl_node_t *node);
static int CmpFunc(const void *num1, const void *num2);
static int PrintNode(void *data, void *dummy);

/********************************FORWARD DECLERATIONS*******************************************/

int main(void)
{
	TestAVL();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/

static void TestAVL(void)
{
	avl_t *new_avl = AVLCreate(CmpFunc);
	int num = 10;
	int num1 = 3;
	int num2 = 7;
	int num3 = 12;
	int num4 = 4;
	int num5 = 9;
	int num6 = 1;
	int num7 = 14;
	int num8 = 11;
	int num9 = 13;
	int num10 = 17;
	int false_num = 132;

	HEADER(AVLCreate);
	TEST(NULL != new_avl, AVLCreate);
	TEST(TRUE == AVLIsEmpty(new_avl), AVLIsEmpty);
	TEST(0 == AVLCount(new_avl), AVLCount);

	HEADER(AVLInsert);
	TEST(SUCCESS == AVLInsert(new_avl, &num), AVLInsert);
	TEST(FALSE == AVLIsEmpty(new_avl), AVLIsNotEmpty);
	AVLInsert(new_avl, &num1);
	AVLInsert(new_avl, &num5);
	AVLInsert(new_avl, &num3);
	AVLInsert(new_avl, &num6);
	AVLInsert(new_avl, &num8);
	AVLInsert(new_avl, &num2);
	AVLInsert(new_avl, &num4);
	AVLInsert(new_avl, &num9);
	AVLInsert(new_avl, &num7);
	AVLInsert(new_avl, &num10);
	PrintTree(new_avl->dummy_root.children[LEFT], 1);
	TEST(SUCCESS == VerifyTree(new_avl->dummy_root.children[LEFT]), VerifyTree);

	HEADER(AVLCount);
	TEST(11 == AVLCount(new_avl), AVLCount);

	HEADER(AVLHeight);
	TEST(4 == AVLHeight(new_avl), AVLHeight);

	HEADER(AVLRemove);
	AVLRemove(new_avl, &num5);
	AVLRemove(new_avl, &num);
	AVLRemove(new_avl, &num2);
	PrintTree(new_avl->dummy_root.children[LEFT], 1);
	TEST(SUCCESS == VerifyTree(new_avl->dummy_root.children[LEFT]), VerifyTree);

	HEADER(AVLFind);
	TEST(SUCCESS == TestFind(new_avl, &num8), AVLFind);
	TEST(NULL == AVLFind(new_avl, &false_num), AVLFind - Fail);

	HEADER(AVLForEach);
	TEST(SUCCESS == AVLForEach(new_avl, PrintNode, &false_num, IN_ORDER), ForEachInOrder);
	TEST(SUCCESS == AVLForEach(new_avl, PrintNode, &false_num, POST_ORDER), ForEachPostOrder);
	TEST(SUCCESS == AVLForEach(new_avl, PrintNode, &false_num, PRE_ORDER), ForEachPreOrder);

	AVLDestroy(new_avl);
}

static int TestFind(avl_t *avl, int *value)
{
	return !(*value == *(int *)AVLFind(avl, value));
}

static int VerifyTree(avl_node_t *node)
{
	int result = SUCCESS;

	if (NULL == node) return result;
	if (NULL != node->children[LEFT])
	{
		result ^= (CmpFunc(node->data, node->children[LEFT]->data) > 0 &&
		VerifyTree(node->children[LEFT]));
	}
	if (NULL != node->children[RIGHT])
	{
		result ^= (CmpFunc(node->data, node->children[RIGHT]->data) < 0 &&
		VerifyTree(node->children[RIGHT]));
	}

	return result;
}

static void PrintTree(avl_node_t *node, int level)
{
	int i = 0;
	if (NULL == node) return;

	PrintTree(node->children[RIGHT], level + 1);
	for (i = 0 ; i < level ; ++i)
	{
        printf("    ");
	}
	printf("%d, h:%ld\n", *(int*)node->data, node->height);
	PrintTree(node->children[LEFT], level + 1);
}

/************************************USER FUNCTIONS**********************************************/
static int CmpFunc(const void *num1, const void *num2)
{
	return *(int *)num1 - *(int *)num2;
}

static int PrintNode(void *data, void *dummy)
{
	(void)dummy;

	printf("%d ",*(int *)data);

	return 0;
}
