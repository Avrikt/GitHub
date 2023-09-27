/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Dave Hairapetian				*
* Date: 23.03.23							*
* Description: Source file for AVL			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "utilities.h"
#include "avl.h"

/********************************FORWARD DECLERATIONS*******************************************/

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

#define ROOT(TREE) (TREE->dummy_root.children[LEFT])
#define RIGHT_CHILD(X) (X->children[RIGHT])
#define LEFT_CHILD(X) (X->children[LEFT])

static avl_node_t *CreateNode(void *data);
static void AVLDestroyRec(avl_node_t *node);
static void AVLInsertRec(avl_node_t *root, int (*cmp_func)(const void *, const void *), avl_node_t *new_node);
static avl_node_t *AVLRemoveRec(avl_node_t *node, int (*cmp_func)(const void *, const void *), const void *key);
static avl_node_t *RemoveNextRec(avl_node_t *node, void **next_data);
static int ForEachInOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);
static int ForEachPreOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);
static int ForEachPostOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param);
static void *AVLFindRec(avl_node_t *node, int (*cmp_func)(const void *, const void *), const void *key);
static size_t AVLCountNodesRec(avl_node_t *node);
static size_t AVLHeightOfNode(avl_node_t *node);
static void UpdateHeight(avl_node_t *node);
static int BalanceFactor(avl_node_t *node);
static void BalanceNode(avl_node_t *node);
static void RotateRight(avl_node_t *node);
static void RotateLeft(avl_node_t *node);
static int IsLeaf(avl_node_t *node);
static int IsOnlyChild(avl_node_t *node);
static avl_node_t *GetSingleChild(avl_node_t *node);
static void Swap(void **ptr1, void **ptr2);

/************************************Functions***************************************************/

avl_t *AVLCreate(int (*cmp_func)(const void *, const void *))
{
    avl_node_t dummy_node = {NULL};
    avl_t *new_avl = NULL;

    assert(NULL != cmp_func);

	new_avl = (avl_t *) malloc(sizeof(avl_t));;
    if (NULL == new_avl)
    {
        return NULL;
    }

    new_avl->cmp_func = cmp_func;
    new_avl->dummy_root = dummy_node;
    new_avl->dummy_root.height = 1;

    return new_avl;
}

void AVLDestroy(avl_t *avl)
{
	AVLDestroyRec(ROOT(avl));
	free(avl);
}

int AVLInsert(avl_t *avl, void *data)
{
    avl_node_t *new_node = NULL;

    assert(NULL != avl);

    new_node = CreateNode(data);
    if(NULL == new_node)
    {
        return FAILURE;
    }

    if(TRUE == AVLIsEmpty(avl))
    {
        ROOT(avl) = new_node;
        return SUCCESS;
    }

    AVLInsertRec(ROOT(avl), avl->cmp_func, new_node);

    return SUCCESS;
}

void AVLRemove(avl_t *avl, const void *key)
{
	assert(NULL != avl);
	assert(NULL != key);

	ROOT(avl) = AVLRemoveRec(ROOT(avl), avl->cmp_func, key);
}

void *AVLFind(const avl_t *avl, const void *key)
{
    assert(NULL != avl);
    assert(NULL != key);

    return AVLFindRec(ROOT(avl), avl->cmp_func, key);
}

int AVLForEach(avl_t *avl, int (*action_func)(void *, void *), void *param, order_t order)
{
    int result = 0;

    assert(NULL != avl);
    assert(NULL != action_func);

    switch (order)
    {
        case(PRE_ORDER):
            result = ForEachPreOrder(ROOT(avl), action_func, param);
            break;
        case(IN_ORDER):
            result = ForEachInOrder(ROOT(avl), action_func, param);
            break;
        case(POST_ORDER):
            result = ForEachPostOrder(ROOT(avl), action_func, param);
            break;
    }

    return result;
}

int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);

    return (NULL == ROOT(avl));
}

size_t AVLCount(const avl_t *avl)
{
    assert(NULL != avl);

    return AVLCountNodesRec(ROOT(avl));
}

size_t AVLHeight(const avl_t *avl)
{
    assert(NULL != avl);

	if(AVLIsEmpty(avl))
	{
		return 0;
	}
    return AVLHeightOfNode(ROOT(avl));
}

/**********************************Static Functions********************************************/

static avl_node_t *CreateNode(void *data)
{
    avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));

    if(NULL == new_node)
    {
        return NULL;
    }

    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->data = data;
    new_node->height = 1;

    return new_node;
}

static void AVLDestroyRec(avl_node_t *node)
{
	if (NULL == node)
	{
		return;
	}

	AVLDestroyRec(LEFT_CHILD(node));
	AVLDestroyRec(RIGHT_CHILD(node));

	free(node);
}

static void AVLInsertRec(avl_node_t *node, int (*cmp_func)(const void *, const void *), avl_node_t *new_node)
{
	int cmp_result = 0;

	cmp_result = (0 < cmp_func(new_node->data, node->data));

	if(NULL == node->children[cmp_result])
	{
		node->children[cmp_result] = new_node;
	}
	else
	{
		AVLInsertRec(node->children[cmp_result], cmp_func, new_node);
	}

	BalanceNode(node);
}

static avl_node_t *AVLRemoveRec(avl_node_t *node, int (*cmp_func)(const void *, const void *), const void *key)
{
	avl_node_t *child = NULL;
	void *next_data = NULL;
	int cmp_result = 0;

	if (NULL == node)
	{
		return NULL;
	}

	cmp_result = cmp_func(key, node->data);

	if (0 == cmp_result)
	{
		if (IsLeaf(node))
		{
			free(node);
			return NULL;
		}
		else if (IsOnlyChild(node))
		{
			child = GetSingleChild(node);
			free(node);
			return child;
		}
		else
		{
			node->children[RIGHT] = RemoveNextRec(node->children[RIGHT], &next_data);
			node->data = next_data;
		}
	}
	else
	{
		node->children[0 < cmp_result] = AVLRemoveRec(node->children[0 < cmp_result], cmp_func, key);
	}

	BalanceNode(node);

	return node;
}

static avl_node_t *RemoveNextRec(avl_node_t *node, void **next_data)
{
	avl_node_t *child = NULL;

	assert(NULL != node);

	if(NULL == node->children[LEFT])
	{
		*next_data = node->data;
		child = RIGHT_CHILD(node);
		free(node);
		return child;
	}

	child = RemoveNextRec(node->children[LEFT], next_data);

	node->children[LEFT] = child;

	BalanceNode(node);

	return node;
}

static void *AVLFindRec(avl_node_t *node, int (*cmp_func)(const void *, const void *), const void *key)
{
    if (NULL == node)
    {
        return NULL;
    }

    if(0 == cmp_func(key, node->data))
    {
        return node->data;
    }

    return AVLFindRec(node->children[0 < cmp_func(key, node->data)], cmp_func, key);
}

static int ForEachInOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
    if(NULL == node)
    {
        return SUCCESS;
    }

    if (SUCCESS == ForEachInOrder(LEFT_CHILD(node), action_func, param) &&
		SUCCESS == action_func(node->data, param) &&
		SUCCESS == ForEachInOrder(RIGHT_CHILD(node), action_func, param))
		{
			return SUCCESS;
		}
    return FAILURE;
}

static int ForEachPreOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
	if(NULL == node)
    {
        return SUCCESS;
    }

	if (SUCCESS == action_func(node->data, param) &&
		SUCCESS == ForEachPreOrder(LEFT_CHILD(node), action_func, param) &&
		SUCCESS == ForEachPreOrder(RIGHT_CHILD(node), action_func, param))
		{
			return SUCCESS;
		}

    return FAILURE;
}

static int ForEachPostOrder(avl_node_t *node, int (*action_func)(void *, void *), void *param)
{
    if(NULL == node)
    {
        return SUCCESS;
    }

    if (SUCCESS == ForEachPostOrder(LEFT_CHILD(node), action_func, param) &&
		SUCCESS == ForEachPostOrder(RIGHT_CHILD(node), action_func, param) &&
    	SUCCESS == action_func(node->data, param))
		{
			return SUCCESS;
		}

    return FAILURE;
}

static void UpdateHeight(avl_node_t *node)
{
	size_t height_left = 0, height_right = 0;

	height_right = (NULL == RIGHT_CHILD(node)) ? 0 : RIGHT_CHILD(node)->height;
	height_left = (NULL == LEFT_CHILD(node)) ? 0 : LEFT_CHILD(node)->height;

	node->height = MAX(height_left, height_right) + 1;
}

static size_t AVLHeightOfNode(avl_node_t *node)
{
	return node->height;
}

static int BalanceFactor(avl_node_t *node)
{
	size_t height_left = 0, height_right = 0;

	height_right = (NULL == RIGHT_CHILD(node)) ? 0 : RIGHT_CHILD(node)->height;
	height_left = (NULL == LEFT_CHILD(node)) ? 0 : LEFT_CHILD(node)->height;

	return height_left - height_right;
}

static void BalanceNode(avl_node_t *node)
{
	UpdateHeight(node);

	if(1 < BalanceFactor(node))
	{
		RotateRight(node);
	}

	if (-1 > BalanceFactor(node))
	{
		RotateLeft(node);
	}
}

static void RotateRight(avl_node_t *node)
{
	avl_node_t *root = NULL;

	if (0 > BalanceFactor(LEFT_CHILD(node)))
	{
		RotateLeft(LEFT_CHILD(node));
	}

	root = LEFT_CHILD(node);
	Swap(&root->data, &node->data);
	LEFT_CHILD(node) = LEFT_CHILD(root);
	LEFT_CHILD(root) = RIGHT_CHILD(root);
	RIGHT_CHILD(root) = RIGHT_CHILD(node);
	RIGHT_CHILD(node) = root;

	UpdateHeight(root);
	UpdateHeight(node);

}

static void RotateLeft(avl_node_t *node)
{
	avl_node_t *root = NULL;

	if (0 < BalanceFactor(RIGHT_CHILD(node)))
	{
		RotateRight(RIGHT_CHILD(node));
	}

	root = RIGHT_CHILD(node);
	Swap(&root->data, &node->data);
	RIGHT_CHILD(node) = RIGHT_CHILD(root);
	RIGHT_CHILD(root) = LEFT_CHILD(root);
	LEFT_CHILD(root) = LEFT_CHILD(node);
	LEFT_CHILD(node) = root;

	UpdateHeight(root);
	UpdateHeight(node);
}

static size_t AVLCountNodesRec(avl_node_t *node)
{
    if(NULL == node)
    {
        return 0 ;
    }

    return (AVLCountNodesRec(LEFT_CHILD(node)) + (AVLCountNodesRec(RIGHT_CHILD(node)))) + 1;
}

static int IsLeaf(avl_node_t *node)
{
    return (1 == node->height);
}

static int IsOnlyChild(avl_node_t *node)
{
    return  (NULL == node->children[LEFT] || NULL == node->children[RIGHT]);
}

static avl_node_t *GetSingleChild(avl_node_t *node)
{
	return (NULL == node->children[LEFT] ? node->children[RIGHT] : node->children[LEFT]);
}

static void Swap(void **ptr1, void **ptr2)
{
	void *tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}
