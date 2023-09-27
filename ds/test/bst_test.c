/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 20.03.23							*
* Description: Test file for BST			*
*********************************************/

/************************************LIBRARIES**************************************************/

#include "utilities.h"
#include "bst.h"

/*********************************GLOBAL VARRIABLES*********************************************/


/********************************FORWARD DECLERATIONS*******************************************/
typedef enum child_pos
{
	LEFT = 0,
	RIGHT = 1,
	NUM_OF_CHILDREN = 2
}child_pos_t;

struct bst_node
{
	bst_node_t *parent;
	bst_node_t *children[NUM_OF_CHILDREN];
	void *data;
};
static void TestBST(void);
static int TestGetData(bst_iter_t iter, int x);
static int TestFind(bst_t *bst, bst_iter_t iter, const void *key);
static int TestNextIter(bst_iter_t iter);
static int TestPrevIter(bst_iter_t iter);
static int VerifyTree(bst_node_t *node);
static int AddToValue(void *a, void *b);
static int CmpFunc(const void *num1, const void *num2);

int main(void)
{
	TestBST();

	return 0;
}

/************************************TEST FUNCTIONS**********************************************/
static void TestBST(void)
{
	bst_t *new_bst = BSTCreate(CmpFunc);
	bst_iter_t test_iter = NULL;
	bst_iter_t root_iter = NULL;
	int num = 10;
	int num1 = 3;
	int num2 = 7;
	int num3 = 12;
	int num4 = 4;
	int num5 = 9;
	int num6 = 1;
	int num7 = 14;
	int num8 = 8;
	int num9 = 6;
	int to_add = 1;

	HEADER(BSTIsEmpty);
	TEST(TRUE == BSTIsEmpty(new_bst), BSTIsEmpty);
	TEST(TRUE == BSTIsSameIter(BSTBeginIter(new_bst), BSTEndIter(new_bst)), BSTIsSameIter);
	TEST(0 == BSTSize(new_bst), BSTSize);

	HEADER(BSTInsert);
	root_iter = BSTInsert(new_bst, &num);
	TEST(SUCCESS == TestGetData(root_iter, num), BSTGetData);

	BSTInsert(new_bst, &num1);
	BSTInsert(new_bst, &num2);
	test_iter = BSTInsert(new_bst, &num3);
	BSTInsert(new_bst, &num4);
	BSTInsert(new_bst, &num5);
	BSTInsert(new_bst, &num6);
	BSTInsert(new_bst, &num7);
	TEST(8 == BSTSize(new_bst), BSTSize);
	TEST(0 == VerifyTree(BSTBeginIter(new_bst)), VerifyTree);

	HEADER(BSTNextIter);
	TEST(SUCCESS == TestNextIter(test_iter),BSTNextIter);

	HEADER(BSTPrevIter);
	TEST(SUCCESS == TestPrevIter(test_iter),BSTPrevIter);

	HEADER(BSTHeight);
	TEST(4 == BSTHeight(new_bst), BSTHeight);
	BSTInsert(new_bst, &num9);
	TEST(5 == BSTHeight(new_bst), BSTHeight);

	HEADER(BSTFind);
	TEST(SUCCESS == TestFind(new_bst, test_iter, &num3), BSTFind);
	TEST(TRUE == BSTIsSameIter(BSTFind(new_bst, &num8), BSTEndIter(new_bst)), BSTFind-Fail);

	HEADER(BSTRemove);
	BSTRemove(test_iter);
	TEST(TRUE == BSTIsSameIter(BSTFind(new_bst, &num3), BSTEndIter(new_bst)), BSTRemove);
	TEST(SUCCESS == VerifyTree(BSTBeginIter(new_bst)), VerifyTree);

	HEADER(ForEach);
	TEST(SUCCESS == BSTForEach(BSTBeginIter(new_bst), BSTEndIter(new_bst), AddToValue, &to_add), BSTForEach);

	BSTDestroy(new_bst);

}

static int TestGetData(bst_iter_t iter, int x)
{
	return !(x == *((int *)BSTGetData(iter)));
}

static int TestFind(bst_t *bst, bst_iter_t iter, const void *key)
{
	return !(BSTIsSameIter(BSTFind(bst, key), iter));
}

static int TestNextIter(bst_iter_t iter)
{
	bst_iter_t test_iter = BSTNextIter(iter);

	return !(*(int *)BSTGetData(iter) < *(int *)BSTGetData(test_iter));
}

static int TestPrevIter(bst_iter_t iter)
{
	bst_iter_t test_iter = BSTPrevIter(iter);

	return !(*(int *)BSTGetData(iter) > *(int *)BSTGetData(test_iter));
}

static int VerifyTree(bst_node_t *node)
{
	int result = 1;
	if (NULL == node) return result;
	if (NULL != node->children[LEFT])
	{
		result &= (CmpFunc(node->data, node->children[LEFT]->data) > 0 &&
		VerifyTree(node->children[LEFT]));
	}
	if (NULL != node->children[RIGHT])
	{
		result &= (CmpFunc(node->data, node->children[RIGHT]->data) < 0 &&
		VerifyTree(node->children[RIGHT]));
	}

	return !result;
}

/************************************USER FUNCTIONS**********************************************/
static int CmpFunc(const void *num1, const void *num2)
{
	return *(int *)num1 - *(int *)num2;
}

static int AddToValue(void *a, void *b)
{
	*(int *)a += *(int *)b;
	return 0;
}
