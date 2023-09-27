/******************************
Developer: Avri Kehat
Reviewed by: Lilach Rabinovich
Date: 30.01.23
Description: Implementation of a stack
*******************************/

#include <stdio.h> /*printf, size_t*/
#include <stdlib.h>	/*malloc, free*/
#include <string.h>	/*memcpy*/
#include <assert.h>	/*assert*/
#include "../include/stack.h"

struct stack 
{
	void *data;
	size_t top;
	size_t capacity;
	size_t i_size;
};

stack_t *StackCreate(size_t capacity, size_t item_size)
{
	stack_t *stack = malloc(sizeof(stack_t));
	if (NULL == stack)
	{
		printf("Malloc failed\n");
		return NULL;	
	}
	stack->data = malloc(capacity * item_size);
	if(NULL == stack->data)
	{
		printf("Malloc failed\n");
		return NULL;					
	}
	stack->top = 0;
	stack->capacity = capacity;
	stack->i_size = item_size; 
	
	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	free(stack->data);
	free(stack);
}

void StackPush(stack_t *stack, const void *item)
{
	assert(NULL != stack);	
	assert(NULL != item);		
	/*assert(stack->top < stack->capacity);*/
	memcpy((char *)stack->data + (stack->i_size * stack->top), item, stack->i_size);
	stack->top += 1;
}

void StackPop(stack_t *stack)
{
	assert(NULL != stack);
	assert(!StackIsEmpty(stack));
	stack->top -= 1; 
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	assert(!StackIsEmpty(stack));
	return ((char *)stack->data + (stack->top - 1) * (stack->i_size));
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	if(0 == stack->top)
	{
		 return 1;
	}
	else
	{
		return 0;
	}
}	

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	return stack->top;
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);
	return stack->capacity;
}












