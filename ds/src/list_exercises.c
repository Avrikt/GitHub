/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 09.02.2023
*********************/

#include <stdio.h> /*printf*/
#include <assert.h>	/*assert*/
#include "list_exercises.h"

static size_t ListLength(node_t *head);

/*Reverse the order of a given slist. */
node_t *Flip(node_t *head)
{
	node_t *curr_node = head;
	node_t *tmp1 = head->next;
	node_t *tmp2 = NULL;
	
	head->next = tmp2;
	
	while (NULL != tmp1->next)
	{
		tmp2 = tmp1->next;
		tmp1->next = curr_node;
		curr_node = tmp1;
		tmp1 = tmp2;		
	}
	tmp1->next = curr_node;
	
	return tmp1;
}

/*Tells whther a given slits has loop.*/
int HasLoop(const node_t *head)
{
	node_t *fast_run = NULL;
	node_t *slow_run = (node_t *)head;
	int succ_flag = 0;
	
	assert(NULL != head->next);
	
	fast_run = slow_run->next;
	
	while(NULL != fast_run->next && NULL != fast_run->next->next)
	{
		fast_run = fast_run->next->next;
		slow_run = slow_run->next;
		if (fast_run == slow_run)
		{
			succ_flag = 1;
			break;
		}
	}
	
	return succ_flag;
}

node_t *FindInterSection(node_t *head_1, node_t *head_2)
{
	node_t *run1 = head_1;
	node_t *run2 = head_2;
	size_t length1 = ListLength(head_1);
	size_t length2 = ListLength(head_2);
	
	while (length1 > length2)
	{
		run1 = run1->next;
		--length1;
	}
	
	while (length2 > length1)
	{
		run2 = run2->next;
		--length2;
	}
	
	while (run2 != run1)
	{
		run1 = run1->next;
		run2 = run2->next;
	}
	return run1;
}

static size_t ListLength(node_t *head)
{
	size_t length = 0;
	assert (NULL != head);
	
	while (NULL != head)
	{
		head = head->next;
		++length;
	}
	return length;
}

void PrintList(node_t *node)
{
	assert(NULL != node);
	
	while (NULL != node->next)
	{
		printf("%d\n", *(int *)node->data);
		node = node->next;
	}
	printf("%d\n", *(int *)node->data);
}
