/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 20.03.23							*
* Description: Source file for BST			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdlib.h> /*malloc, free*/

#include "bst.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/

#define ROOT(BST) (BST->dummy_root.children[LEFT])

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

struct binary_search_tree
{
	bst_node_t dummy_root;
	int (*cmp_func)(const void *, const void *);
};

static bst_iter_t CreateNode(void *data);
static int IsLeaf(bst_iter_t iter);
static int IsOnlyChild(bst_iter_t iter);
static bst_iter_t GetSingleChild(bst_iter_t iter);
static child_pos_t ChildPosition(bst_iter_t iter);
static bst_iter_t TreeTraverse(bst_iter_t iter, const child_pos_t side);
static size_t BSTHeightOfSubTree(bst_iter_t node);
static bst_iter_t RemoveMiddleNode(bst_iter_t iter);

/************************************Functions***************************************************/

bst_t *BSTCreate(int (*cmp_func)(const void *, const void *))
{
    bst_node_t dummy_node = {NULL};
    bst_t *new_bst = NULL;

    ASSERT(NULL != cmp_func);

	new_bst = (bst_t *) malloc(sizeof(bst_t));
    if (NULL == new_bst)
    {
        return NULL;
    }

    new_bst->cmp_func = cmp_func;
    new_bst->dummy_root = dummy_node;

    return new_bst;
}

void BSTDestroy(bst_t *bst)
{
	bst_iter_t runner = NULL;
	bst_iter_t to_remove =NULL;

	ASSERT(NULL != bst);

	to_remove = BSTBeginIter(bst);

    while (FALSE == BSTIsEmpty(bst))
    {
		runner = BSTNextIter(to_remove);
		BSTRemove(to_remove);
		to_remove = runner;
    }

    free(bst);
}

bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    bst_iter_t new_node = NULL;
    bst_iter_t tree_runner = NULL;
    bst_iter_t parent = NULL;
    child_pos_t child_pos = 0;

    ASSERT(NULL != bst);

    new_node = CreateNode(data);
    if(NULL == new_node)
    {
        return BSTEndIter(bst);
    }

    tree_runner = ROOT(bst);
    parent = BSTEndIter(bst);

    while (NULL != tree_runner)
    {
        parent = tree_runner;
		child_pos = 0 < bst->cmp_func(data, tree_runner->data);
		tree_runner = tree_runner->children[child_pos];
    }

    new_node->parent = parent;
    parent->children[child_pos] = new_node;

    return new_node;
}

void BSTRemove(bst_iter_t iter)
{
    bst_iter_t parent = NULL;
    bst_iter_t child = NULL;
    child_pos_t position = 0;

    ASSERT(NULL != iter);

	position = ChildPosition(iter);
    parent = iter->parent;

    if (TRUE == IsLeaf(iter))
    {
        parent->children[position] = NULL;
    }
    else if (TRUE == IsOnlyChild(iter))
    {
        child = GetSingleChild(iter);
        parent->children[position] = child;
        child->parent = parent;
    }
    else
    {
		iter = RemoveMiddleNode(iter);
    }

    free(iter);
}

bst_iter_t BSTFind(const bst_t *bst, const void *key)
{
	bst_iter_t runner = NULL;
	int cmp_result = 0;

	ASSERT(NULL != bst);

	runner = ROOT(bst);
	cmp_result = bst->cmp_func(key, runner->data);

	while(cmp_result != 0)
	{
		runner = runner->children[cmp_result > 0];
		if (NULL == runner)
		{
			runner = BSTEndIter(bst);
			break;
		}
		cmp_result = bst->cmp_func(key, runner->data);
	}

	return runner;
}

int BSTForEach(bst_iter_t from, const bst_iter_t to, int (*action_func)(void *, void *), void *param)
{
	bst_iter_t runner = from;
	int success_flag = 0;

	ASSERT(NULL != from);
	ASSERT(NULL != to);
	ASSERT(NULL != action_func);

	while (FALSE == BSTIsSameIter(runner, to) && SUCCESS == success_flag)
	{
		success_flag = action_func(runner->data, param);
		runner = BSTNextIter(runner);
	}

	return success_flag;
}

int BSTIsEmpty(const bst_t *bst)
{
    ASSERT(NULL != bst);

    return (NULL == ROOT(bst));
}

size_t BSTSize(const bst_t *bst)
{
    size_t size = 0;
    bst_iter_t traverse_iter = NULL;

    ASSERT(NULL != bst);

    traverse_iter = BSTBeginIter(bst);

    while (FALSE == BSTIsSameIter(traverse_iter, BSTEndIter(bst)))
    {
        ++size;
        traverse_iter = BSTNextIter(traverse_iter);
    }

    return size;
}

void *BSTGetData(bst_iter_t iter)
{
    ASSERT(NULL != iter);

    return iter->data;
}

size_t BSTHeight(const bst_t *bst)
{
	return BSTHeightOfSubTree(ROOT(bst));
}

/*****************************Iterator Functions *******************************************/

bst_iter_t BSTBeginIter(const bst_t *bst)
{
    bst_iter_t iter = NULL;

    ASSERT(NULL != bst);

    iter = (bst_iter_t)&bst->dummy_root;

    while (NULL != iter->children[LEFT])
    {
        iter = iter->children[LEFT];
    }

    return iter;
}

bst_iter_t BSTEndIter(const bst_t *bst)
{
    ASSERT(NULL != bst);

    return (bst_iter_t)(&bst->dummy_root);
}

bst_iter_t BSTNextIter(const bst_iter_t iter)
{
    ASSERT(NULL != iter);

    return TreeTraverse(iter, RIGHT);
}

bst_iter_t BSTPrevIter(const bst_iter_t iter)
{
    ASSERT(NULL != iter);

    return TreeTraverse(iter, LEFT);
}

int BSTIsSameIter(const bst_iter_t iter1, const bst_iter_t iter2)
{
    ASSERT(NULL != iter1);
    ASSERT(NULL != iter2);

    return (iter1 == iter2);
}

/**********************************Static Functions********************************************/

static bst_iter_t CreateNode(void *data)
{
    bst_iter_t new_node = (bst_iter_t)malloc(sizeof(bst_node_t));

    if(NULL == new_node)
    {
        return NULL;
    }

    new_node->parent = NULL;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->data = data;

    return new_node;
}

static int IsLeaf(bst_iter_t iter)
{
    return (NULL == iter->children[LEFT] && NULL == iter->children[RIGHT]);
}

static int IsOnlyChild(bst_iter_t iter)
{
    return  (NULL == iter->children[LEFT] || NULL == iter->children[RIGHT]);
}

static bst_iter_t GetSingleChild(bst_iter_t iter)
{
    return (NULL == iter->children[LEFT] ? iter->children[RIGHT] : iter->children[LEFT]);
}

static child_pos_t ChildPosition(bst_iter_t iter)
{
    return (iter == iter->parent->children[RIGHT] ? RIGHT : LEFT);
}

static bst_iter_t RemoveMiddleNode(bst_iter_t iter)
{
	bst_iter_t parent = NULL;
	bst_iter_t child = NULL;
	bst_iter_t runner =  BSTNextIter(iter);

	iter->data = runner->data;
	child = runner->children[RIGHT];
	parent = runner->parent;
	parent->children[ChildPosition(runner)] = child;

	if(NULL != child)
	{
		child->parent = parent;
	}
	return runner;
}

static bst_iter_t TreeTraverse(bst_iter_t iter, const child_pos_t side)
{
    bst_iter_t iter_run = NULL;
    bst_iter_t parent = NULL;

    if (NULL != iter->children[side])
    {
        iter_run = iter->children[side];
        while (NULL != iter_run->children[!side])
        {
            iter_run = iter_run->children[!side];
        }
    }
	else
	{
	    parent = iter->parent;
        iter_run = iter;
        while (NULL != parent && iter_run == parent->children[side])
        {
            iter_run = parent;
            parent = parent->parent;
        }
		iter_run = parent;
	}

    return iter_run;
}

static size_t BSTHeightOfSubTree(bst_iter_t node)
{
	if (NULL == node)
	{
		return 0;
	}

	return 	MAX(BSTHeightOfSubTree(node->children[RIGHT]),
				BSTHeightOfSubTree(node->children[LEFT])) + 1;
}
