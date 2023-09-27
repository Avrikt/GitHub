/********************
Developer: Avri Kehat
Reviewed by: Yossi Matzliah
Date: 15.02.23
Description: Implementation of C_buffer
*********************/

#include <stdio.h> 		/*perror*/
#include <assert.h>		/*assert*/
#include <stdlib.h>		/*malloc, free*/
#include <errno.h>		/*errno*/
#include "c_buffer.h"

struct c_buffer
{
	size_t capacity;
	size_t read;
	size_t write;
	int is_empty;	
	char data[1];
};

c_buffer_t *CBufferCreate(size_t capacity)
{
	c_buffer_t *new_buffer = (c_buffer_t *) malloc(sizeof(c_buffer_t) + capacity);								
	
	if(NULL == new_buffer)
	{
		perror("Error");
		return NULL;
	}
	
	new_buffer->capacity = capacity;
	new_buffer->read = 0;
	new_buffer->write = 0;
	new_buffer->is_empty = 1;
	
	return new_buffer;
}

void CBufferDestroy(c_buffer_t *buffer)
{
	assert (NULL != buffer);
	free(buffer);
}							

ssize_t CBufferWrite(c_buffer_t *buffer,const void *src, size_t data_size)
{
	size_t i = 0;
	size_t bytes_to_write = CBufferFreeSpace(buffer);

	if (0 == bytes_to_write)
	{
		errno = ECANCELED;
		return -1;
	}
	
	if (data_size > bytes_to_write)
	{
		data_size = bytes_to_write;
		errno = ENOMEM;
	}
	
	buffer->is_empty = 0;
	
	for(i = 0; data_size > i; ++i)
	{
		if (buffer->write == buffer->capacity)
		{
			buffer->write = 0;
		}
		buffer->data[buffer->write] = *(char *)src + i;	
		++buffer->write;
	}
	return i;
}

ssize_t CBufferRead(c_buffer_t *buffer ,void *dest ,size_t size)
{
	size_t i = 0;
	size_t bytes_to_read = CBufferSize(buffer);
	
	if (size > bytes_to_read)
	{
		size = bytes_to_read;
		errno = EINTR;
	}
	
	for (i = 0; size > i; ++i)
	{
		if (buffer->read == buffer->capacity)
		{
			buffer->read = 0;
		}
		*((char *)dest + i) = buffer->data[buffer->read];
		++buffer->read;
		
		if(buffer->read == buffer->write)
		{	
			buffer->is_empty = 1;
		}
	}
	return i;
}	

size_t CBufferSize(const c_buffer_t *buffer)
{
	size_t size_of_buffer = 0;
	assert (NULL != buffer);
	
	if (buffer->write > buffer->read)
	{
		size_of_buffer = buffer->write - buffer->read;
	}
	else if ((buffer->write == buffer->read) && (1 == buffer->is_empty))
	{
		size_of_buffer = 0;
	}
	else
	{
		size_of_buffer = buffer->capacity - (buffer->read - buffer->write);
	}
	return size_of_buffer;
}								

size_t CBufferFreeSpace(const c_buffer_t *buffer)
{
	return (buffer->capacity - CBufferSize(buffer));						
}

int CBufferIsEmpty(const c_buffer_t *buffer)
{
	return (0 == CBufferSize(buffer));
}

