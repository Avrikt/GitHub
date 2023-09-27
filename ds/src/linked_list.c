/********************
Developer: Avri Kehat
Reviewed by: Oran Frieden
Date: 08.02.23
*********************/

#include <stdio.h> /*perror, size_t*/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include "linked_list.h"

struct node
{
    void *data;
    node_t *next;
};

struct SingleLinkedList
{
	node_t *head;
	node_t *tail;
};

int ListCounter(void *dummy, void *count);

sll_t *SLLCreate()
{
	sll_t *new_list = NULL;
	iterator_t dummy = NULL;
	
	new_list = (sll_t *) malloc(sizeof(sll_t));

	if(NULL == new_list)
	{
		perror("Error");
		return NULL;
	}
	dummy = (node_t *) malloc(sizeof(node_t));
	if (NULL == dummy)
	{
		perror("Error");
		free(new_list);
		return NULL;
	}
	
	dummy->data = (sll_t *)new_list;
	dummy->next = NULL;
	
	new_list->head = dummy;
	new_list->tail = dummy;

	return new_list;
}

void SLLDestroy(sll_t *sll)
{
	iterator_t curr_node = NULL;

	assert(NULL != sll);
		
	while(NULL != sll->head)
	{
		curr_node = sll->head;
		sll->head = sll->head->next;
		free(curr_node);
	}
	free(sll);
}

iterator_t SLLBeginIter(const sll_t *sll)
{
	assert(NULL != sll);
	return sll->head;
}

iterator_t SLLNextIter(const iterator_t iterator)
{
	assert(NULL != iterator);
	return iterator->next;
}

iterator_t SLLEndIter(const sll_t *sll)
{
	assert(NULL != sll);
	return sll->tail;
}

int SLLIsSameIter(const iterator_t iter1, const iterator_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return(iter1 == iter2);
}
/************************** User Functions ************************/

size_t SLLCount(const sll_t *sll)
{
	size_t count = 0;

	assert(NULL != sll);
	
	SLLForEach(SLLBeginIter(sll), SLLEndIter(sll), &ListCounter, &count);

	return count;
}

iterator_t SLLInsertBefore(iterator_t iterator, void *data)
{
	iterator_t tmp_node = NULL;
	assert(NULL != iterator);
	assert(NULL != data);
	
	tmp_node = (iterator_t) malloc(sizeof(node_t)); 

	if(NULL == tmp_node)
	{
		perror("Error");
		return NULL;
	}

	tmp_node->data = iterator->data;
	tmp_node->next = iterator->next;
	
	iterator->data = data;
	iterator->next = tmp_node;
	
	if(NULL == tmp_node->next)
	{
		((sll_t *)tmp_node->data)->tail = tmp_node;
	}
	
	return iterator;
}		

iterator_t SLLRemove(iterator_t iterator)
{
	iterator_t tmp_iter = NULL;
	assert(NULL != iterator);
	
	tmp_iter = iterator->next;
	
	iterator->data = tmp_iter->data;
	iterator->next = tmp_iter->next;
	
	free(tmp_iter);
	
	return iterator;
}

iterator_t SLLFind(iterator_t from, iterator_t to, is_match user_func, void *param)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != user_func);
	
	while((from != to) && (0 != user_func(from->data, param)))
	{
		from = from->next;
	}
	
	return from;
}

void SLLSetData(const iterator_t iterator, void *data)
{
	assert(NULL != iterator);
	assert(NULL != data);
	
	iterator->data = data;
}

void *SLLGetData(iterator_t iterator)
{
	assert(NULL != iterator);
	return iterator->data;
}

int SLLForEach(iterator_t from, const iterator_t to, action_func user_func, void *param)
{
	int func_flag = 0;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != param);
	assert(NULL != user_func);
	
	while((to != from) && (0 == func_flag))
	{
		func_flag = user_func(from->data, param);
		from = from->next;
	}
	return func_flag;
}

int SLLIsEmpty(const sll_t *sll)
{
	assert(NULL != sll);
	return (NULL == sll->head->next);
}

sll_t *SLLAppend(sll_t *dest, sll_t *src)
{
	assert (NULL != dest);
	assert (NULL != src);
	
	dest->tail->data = src->head->data;
	dest->tail->next = src->head->next;
		
	src->tail->data = (void *)dest;
	
	src->head->next = NULL;
	src->head->data = (void *)src;
	
	dest->tail = src->tail;
	src->tail = src->head;
	
	return dest;
}

int ListCounter(void *dummy, void *count)
{
	++*(size_t *)count;
	(void) dummy;
	return 0;
}
