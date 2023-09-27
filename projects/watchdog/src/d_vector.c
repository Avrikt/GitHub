/********************
Developer: Avri Kehat
Reviewed by: Omri Shalev
Date: 03.02.23
*********************/

#include <stdio.h> 	/*printf, size_t*/
#include <stdlib.h>	/*malloc, realloc, free*/
#include <assert.h>	/*assert*/
#include <string.h> /*memset*/

#include "d_vector.h"

#define GROWTH_FACTOR 2

struct dynamic_vector
{
	void *data;
	size_t size;
	size_t capacity;
	size_t element_size;
};

vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	vector_t *vector = malloc(sizeof(vector_t));	
	if(NULL == vector)
	{
		return NULL;
	}
	
	vector->data = malloc(capacity * element_size);
	if(NULL == vector->data)
	{
		free(vector);
		return NULL;
	}
	
	vector->size = 0;
	vector->capacity = capacity;
	vector->element_size = element_size;
	
	return vector;
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	memset(vector->data, 0, (vector->capacity * vector->element_size));
	free(vector->data);
	free(vector);
}

int VectorPushBack(vector_t *vector, const void *element)
{
	int realloc_fail_flag = 0;
		
	assert(NULL != vector);
	assert(NULL != element);
	 	
	if (vector->size == vector->capacity - 1)
	{
		realloc_fail_flag = VectorReserve(vector, (vector->capacity * GROWTH_FACTOR));
	}
	
	memcpy((char *)vector->data + (vector->size * vector->element_size), element, vector->element_size);
	
	++vector->size;
		
	return realloc_fail_flag;
}
	
void VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);
	assert(!VectorIsEmpty(vector));
	
	--vector->size;
						
	if(vector->size == (vector->capacity / 4))
	{
		vector->data = realloc(vector->data, ((vector->capacity * vector->element_size) / GROWTH_FACTOR));
		vector->capacity /= GROWTH_FACTOR;
	}
}

void *VectorGetAccessToElement(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	return ((char *)vector->data + (index * vector->element_size));
}

int VectorReserve(vector_t *vector, size_t reserve_size)
{
	void *realloc_ptr = NULL;
	assert(NULL != vector);	

	if (reserve_size < vector->capacity)
	{
		return 0;
	}
	
	realloc_ptr = realloc(vector->data, (reserve_size * vector->element_size));
	
	if (NULL == realloc_ptr)
	{
		perror("Error: ");
		return 1;
	}
	
	vector->data = realloc_ptr;
	vector->capacity = reserve_size;
	
	return 0;
}

int VectorShrinkToSize(vector_t *vector)
{
	void *realloc_ptr = NULL;
	assert(NULL != vector);
	
	realloc_ptr = realloc(vector->data, (vector->size * (vector->element_size + 1)));
	
	if(NULL == realloc_ptr)
	{
		perror("Error: ");
		return 1;
	}
	
	vector->data = realloc_ptr;
	vector->capacity = vector->size + 1;
	return 0;
}

int VectorIsEmpty(const vector_t *vector)
{
	assert(NULL != vector);
	return (0 == vector->size);
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	return vector->capacity;
}

size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);
	return vector->size;
}
