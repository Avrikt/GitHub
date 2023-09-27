#ifndef MY_PROJECT_C_BUFFER_H__
#define MY_PROJECT_C_BUFFER_H__

#include <sys/types.h>

typedef struct c_buffer c_buffer_t;

/*
* CBufferCreate description:
* 	creates a circular buffer data structure			
*
* @param:
* 	capacity - the capacity of elements the c_buffer will contain.
* 
* @return:
* 	returns a pointer to the created buffer.
*
* complexcity
* 	O(1)
*
*/
c_buffer_t *CBufferCreate(size_t capacity);									


/*
* CBufferDestroy description:
* 	destroys a given circular buffer				
*
* @param:
* 	buffer - a pointer to the circular buffer to be destroyed.
* 
* @return:
* 	non
*
* complexcity
* 	O(1)
*/
void CBufferDestroy(c_buffer_t *buffer);


/*
* CBufferWrite description:
* 	Writes an element to the end of the given buffer.
* 	Writing to a full buffer will lead to an undefined behavior.
*
* @param:
* 	buffer - a pointer to the circular buffer to write the data to.
* 	src - a pointer to the buffer to copy the data needs to be writen from.
* 	data_size - size of the data needs to be writen.
*
* @return:
* 	the number of elements succesfuly writen.
*
* complexcity
* 	O(n)
*/
ssize_t CBufferWrite(c_buffer_t *buffer,const void *src, size_t data_size); 


/*
* CBufferRead description:
* 	Read the first element from the given buffer.
* 	Reading from an empty buffer will lead to undefined behavior
* @param:
* 	buffer - a pointer to the circular buffer to read the data from.
* 	dest - a poiner to the buffer to store the data needs to be read in.
* 	data_size - size of the data needs to be read.
*
* @return:
* 	the number of elements succesfuly read.
*
* complexcity
* 	O(1)
*/
ssize_t CBufferRead(c_buffer_t *buffer,void *dest, size_t size);


/*
* CBufferSize description:
* 	Returns the number of elements that are currently in the buffer.
* 	Reading from an empty buffer will lead to an undefined behavior.
*
* @param:
* 	buffer - a pointer to the circular buffer.
*
* @return:
* 	Returns the number of elements that are currently in the buffer.
*
* complexcity
* 	O(1)
*/
size_t CBufferSize(const c_buffer_t *buffer);


/*
* CBufferFreeSpace description:
* 	Returns how much free space left in the buffer.
*
* @param:
* 	buffer - a pointer to the circular buffer.
*
* @return:
* 	Returns how much free space was left in the buffer.
*
* complexcity
* 	O(1)
*/ 
size_t CBufferFreeSpace(const c_buffer_t *buffer);


/*
* CBufferIsEmpty description:
* 	Checks if the buffer given is empty or not.
*
* @param:
* 	buffer - a pointer to the circular buffer.
*
* @return:
* 	Returns 1 if the given buffer is empty and 0 if not.
*
* complexcity
* 	O(1)
*/ 
int CBufferIsEmpty(const c_buffer_t *buffer);

#endif
