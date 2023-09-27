/******************************
Developer: Avri Kehat
Reviewed by: Lilach Rabinovich
Date: 19.02.2023
*******************************/

#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "sorted_list.h"

struct sorted_linked_list
{
	dll_t *list;
	cmp_func_st user_func;
};

typedef int(*function_cmp)(const int);

static int WrapCompareFunction(const int difference);
static int WrapIsBiggerFunction(const int difference);
static sol_iterator_t GeneralFind(sol_t *sol, sol_iterator_t from, sol_iterator_t to, const void *to_find, function_cmp function);

/******************************functions***********************************************/

sol_t *SortedListCreate(cmp_func_st function)
{
	sol_t *new_list = NULL;
	assert (NULL != function);
	new_list = (sol_t *)malloc(sizeof(sol_t));

	if (NULL == new_list)
	{
		return NULL;
	}

	new_list->list = DLLCreate();

	if (NULL == new_list->list)
	{
		free(new_list);
		return NULL;
	}
	new_list->user_func = function;

	return new_list;
}

void SortedListDestroy(sol_t *sol)
{
	assert (NULL != sol);
	DLLDestroy(sol->list);
	free(sol);
}

int SortedListIsEmpty(const sol_t *sol)
{
	assert (NULL != sol);

	return DLLIsEmpty(sol->list);
}

size_t SortedListSize(const sol_t *sol)
{
	assert (NULL != sol);

	return DLLCount(sol->list);
}

sol_iterator_t SortedListInsert(sol_t *sol, void *data)
{
	sol_iterator_t to_insert;
	assert (NULL != sol);

	to_insert = GeneralFind(sol, SortedListBeginIter(sol), SortedListEndIter(sol), data, WrapIsBiggerFunction);

	to_insert.dll_iterator = DLLInsert(to_insert.dll_iterator, data);

	#ifndef NDEBUG
	to_insert.dll = sol->list;
	#endif

	return to_insert;
}

sol_iterator_t SortedListRemove(sol_iterator_t iterator)
{
	sol_iterator_t to_remove;
	to_remove.dll_iterator = DLLRemove(iterator.dll_iterator);

	return to_remove;
}

void *SortedListPopBack(sol_t *sol)
{
	return DLLPopBack(sol->list);
}

void *SortedListPopFront(sol_t *sol)
{
	return DLLPopFront(sol->list);
}

sol_iterator_t SortedListFindIf(sol_iterator_t from, sol_iterator_t to, is_match_t user_func, void *param)
{
	assert (from.dll == to.dll);

	from.dll_iterator = DLLFind(from.dll_iterator, to.dll_iterator, user_func, param);

	return from;
}

sol_iterator_t SortedListFind(sol_t *sol, sol_iterator_t from, sol_iterator_t to, const void *to_find)
{
	return GeneralFind(sol, from, to, to_find, WrapCompareFunction);
}

int SortedListForEach(sol_iterator_t from, sol_iterator_t to, action_func_t user_func, void *param)
{
	assert (from.dll == to.dll);

	return DLLForEach(from.dll_iterator, to.dll_iterator, user_func, param);
}

void *SortedListGetData(sol_iterator_t iterator)
{
	return DLLGetData(iterator.dll_iterator);
}

sol_iterator_t SortedListBeginIter(const sol_t *sol)
{
	sol_iterator_t iter;
	assert (NULL != sol);

	iter.dll_iterator =  DLLBeginIter(sol->list);

	#ifndef NDEBUG
	iter.dll = sol->list;
	#endif

	return iter;
}

sol_iterator_t SortedListEndIter(const sol_t *sol)
{
	sol_iterator_t iter;
	assert (NULL != sol);

	iter.dll_iterator =  DLLEndIter(sol->list);

	#ifndef NDEBUG
	iter.dll = sol->list;
	#endif

	return iter;
}

sol_iterator_t SortedListNextIter(sol_iterator_t iterator)
{
	iterator.dll_iterator = DLLNextIter(iterator.dll_iterator);

	return iterator;
}

sol_iterator_t SortedListPrevIter(sol_iterator_t iterator)
{
	iterator.dll_iterator = DLLPrevIter(iterator.dll_iterator);

	return iterator;
}

int SortedListIsSameIter(sol_iterator_t iter1, sol_iterator_t iter2)
{
	assert (iter1.dll == iter2.dll);

	return DLLIsSameIter(iter1.dll_iterator, iter2.dll_iterator);
}

sol_t *SortedListMerge(sol_t *dest_sol, sol_t *src_sol)
{
	sol_iterator_t to_find = {0};
	sol_iterator_t from_iter = {0};
	sol_iterator_t to_iter = {0};
	sol_iterator_t to_merge = {0};

	assert (NULL != dest_sol);
	assert (NULL != src_sol);

	from_iter = SortedListBeginIter(dest_sol);
	to_iter = SortedListEndIter(dest_sol);
	to_merge = SortedListBeginIter(src_sol);

	while (!SortedListIsEmpty(src_sol))
	{
		to_find = GeneralFind(dest_sol, from_iter, to_iter, SortedListGetData(to_merge), WrapIsBiggerFunction);
		DLLSplice(to_find.dll_iterator, to_merge.dll_iterator, DLLNextIter(to_merge.dll_iterator));
		to_merge = SortedListBeginIter(src_sol);
		from_iter = to_find;
	}

	return dest_sol;

}

static sol_iterator_t GeneralFind(sol_t *sol, sol_iterator_t from, sol_iterator_t to, const void *to_find, function_cmp is_true_function)
{
	assert (from.dll == to.dll);
	assert(NULL != sol);
	assert(NULL != to_find);

	while ((0 == SortedListIsSameIter(to, from)) && (1 == is_true_function(sol->user_func(SortedListGetData(from), to_find))))
	{
		from = SortedListNextIter(from);
	}

	return from;
}

static int WrapCompareFunction(const int difference)
{
	return (0 == difference);
}

static int WrapIsBiggerFunction(const int difference)
{
	return (0 >= difference);
}
