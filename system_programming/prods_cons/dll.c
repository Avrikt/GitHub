/********************
Developer: Avri Kehat
Reviewed by: Noam Gazit
Date: 16.02.2023
Description: Implementation of Doubly Linked List
*********************/

#include <stdio.h> /*perror, size_t*/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <errno.h> /*errno*/
#include "dll.h"

struct node
{
	node_t *prev;
	void *data;
	node_t *next;
};

struct doubly_linked_list
{
	node_t *head;
	node_t *tail;
};

static int TableCounter(void *dummy, void *count);

dll_t *DLLCreate()
{
	dll_t *new_list = NULL;

	new_list = (dll_t *) malloc(sizeof(dll_t));

	if(NULL == new_list)
	{
		return NULL;
	}

	new_list->head = (node_t *) malloc(sizeof(node_t));
	if (NULL == new_list->head)
	{
		free(new_list);
		return NULL;
	}

	new_list->tail = (node_t *) malloc(sizeof(node_t));

	if (NULL == new_list->tail)
	{
		free(new_list->head);
		free(new_list);
		return NULL;
	}

	new_list->head->prev = NULL;
	new_list->head->data = new_list;
	new_list->head->next = new_list->tail;

	new_list->tail->prev = new_list->head;
	new_list->tail->data = new_list;
	new_list->tail->next = NULL;

	return new_list;
}

void DLLDestroy(dll_t *dll)
{
	iterator_t to_remove = NULL;

	while (NULL != dll->head)
	{
		to_remove = dll->head;
		dll->head = dll->head->next;
		free(to_remove);
	}
	free(dll);
}

int DLLIsEmpty(const dll_t *dll)
{
	return (dll->head->next == dll->tail);
}

size_t DLLCount(const dll_t *dll)
{
	size_t count = 0;

	assert(NULL != dll);

	DLLForEach(DLLBeginIter(dll), DLLEndIter(dll), TableCounter, &count);

	return count;
}

iterator_t DLLInsert(iterator_t iterator, void *data)
{
	iterator_t new_node = NULL;
	assert(NULL != iterator);
	assert(NULL != data);

	new_node = (iterator_t) malloc(sizeof(node_t));

	if(NULL == new_node)
	{
		while (NULL != iterator->next)
		{
			iterator = iterator->next;
		}
		return iterator;
	}

	new_node->prev = iterator->prev;
	new_node->data = data;
	new_node->next = iterator;

	iterator->prev->next = new_node;
	iterator->prev = new_node;

	return new_node;
}

iterator_t DLLRemove(iterator_t iterator)
{
	iterator_t next_iter = NULL;
	iterator_t prev_iter = NULL;

	assert(NULL != iterator);
	assert(NULL != iterator->next);

	next_iter = DLLNextIter(iterator);
	prev_iter = DLLPrevIter(iterator);

	if(next_iter != NULL)
	{
		next_iter->prev = prev_iter;
	}
	prev_iter->next = next_iter;
	free(iterator);
	return next_iter;
}

iterator_t DLLPushBack(dll_t *dll, void *data)
{
	assert (NULL != dll);
	assert (NULL != data);

	return DLLInsert(DLLEndIter(dll), data);
}

iterator_t DLLPushFront(dll_t *dll, void *data)
{
	assert (NULL != dll);
	assert (NULL != data);

	return DLLInsert(DLLBeginIter(dll), data);
}

void *DLLPopBack(dll_t *dll)
{
	void *data = NULL;
	iterator_t to_remove = NULL;
	assert (NULL != dll);

	to_remove = DLLPrevIter(DLLEndIter(dll));
	data = to_remove->data;
	DLLRemove(to_remove);

	return data;
}

void *DLLPopFront(dll_t *dll)
{
	void *data = NULL;
	iterator_t to_remove = NULL;
	assert (NULL != dll);

	to_remove =  DLLBeginIter(dll);
	data = to_remove->data;
	DLLRemove(to_remove);

	return data;
}

iterator_t DLLFind(iterator_t from, iterator_t to, is_match_t user_func, const void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != user_func);

	while(0 == DLLIsSameIter(from, to) && (0 == user_func(from->data, param)))
	{
		from = from->next;
	}
	return from;
}

dll_t *DLLMultiFind(dll_t *dll_dest, iterator_t from, iterator_t to, is_match_t user_func, void *param)
{
	iterator_t test_iter = NULL;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != user_func);

	while(from != to)
	{
		if(0 != user_func(from->data, param))
		{
			test_iter = DLLPushBack(dll_dest, from->data);
			if (NULL == test_iter)
			{
				return NULL;
			}
		}
		from = from->next;
	}

	return dll_dest;
}

int DLLForEach(iterator_t from, const iterator_t to, action_func_t user_func, void *param)
{
	int func_flag = 0;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != user_func);

	while(0 == DLLIsSameIter(from, to) && (0 == func_flag))
	{
		func_flag = user_func(from->data, param);
		from = from->next;
	}
	return func_flag;
}

void DLLSetData(const iterator_t iterator, void *data)
{
	assert(NULL != iterator);
	assert(NULL != data);

	iterator->data = data;
}

void *DLLGetData(iterator_t iterator)
{
	assert(NULL != iterator);
	return iterator->data;
}

iterator_t DLLBeginIter(const dll_t *dll)
{
	assert (NULL != dll);
	return dll->head->next;
}

iterator_t DLLEndIter(const dll_t *dll)
{
	assert(NULL != dll);
	return dll->tail;
}

iterator_t DLLNextIter(const iterator_t iterator)
{
	assert(NULL != iterator);
	return iterator->next;
}

iterator_t DLLPrevIter(const iterator_t iterator)
{
	assert(NULL != iterator);
	return iterator->prev;
}

int DLLIsSameIter(const iterator_t iter1, const iterator_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);

	return(iter1 == iter2);
}

iterator_t DLLSplice(iterator_t dest, iterator_t src_from ,iterator_t src_to)
{
	iterator_t prev = NULL;
	iterator_t tmp = NULL;

	prev = DLLPrevIter(src_from);
	tmp = DLLPrevIter(src_to);
	prev->next = src_to;
	src_to->prev = prev;

	prev = DLLPrevIter(dest);
	prev->next = src_from;
	src_from->prev = prev;

	tmp->next = dest;
	dest->prev = tmp;

	return src_from;
}

static int TableCounter(void *dummy, void *count)
{
	++*(size_t *)count;
	(void) dummy;
	return 0;
}
