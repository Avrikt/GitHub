/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 07.03.23							*
* Description: Source file for FSA			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <assert.h> /*assert*/

#include "fsa.h"

#define WORDSIZE (sizeof(size_t))
/***************************PROTOTYPES AND GLOBAL VARIABLES**************************************/

struct fsa
{
    size_t next_block;
};
/************************************Functions***************************************************/

fsa_t *FSAInit(void *memory_segment, size_t segment_size, size_t block_size)
{
    fsa_t *fsa = NULL;
	size_t i = 0;
	size_t num_of_blocks = 0;
    size_t remain =((size_t) memory_segment % WORDSIZE);

	assert(NULL != memory_segment);
	assert(0 != segment_size);
	assert(0 != block_size);

    if (0 != remain)
    {
        memory_segment = (size_t *)((size_t)memory_segment + WORDSIZE - remain);
        segment_size -= remain;
    }
	fsa = (fsa_t *)memory_segment;
	fsa->next_block = sizeof(fsa_t);

	if (0 != block_size % WORDSIZE)
	{
		block_size += WORDSIZE - (block_size % WORDSIZE);
	}

	num_of_blocks = segment_size / block_size;

	for (i = 0; i < num_of_blocks - 1; ++i)
	{
		*(size_t *)((char *)fsa + fsa->next_block + (i * block_size)) = (size_t)(((i + 1) * block_size) + fsa->next_block);
	}

	*(size_t *)((char *)fsa + segment_size - block_size) = 0;

	return fsa;

}

void *FSAAlloc(fsa_t *fsa)
{
    void *segment = NULL;

    assert (NULL != fsa);

    if (0 != fsa->next_block)
    {
        segment = (void *)((char *)fsa + fsa->next_block);
        fsa->next_block = *(size_t *)segment;
    }

    return segment;
}

void FSAFree(fsa_t *fsa, void *block_pointer)
{
    assert (NULL != fsa);
    assert (NULL != block_pointer);

    *(size_t *)block_pointer = fsa->next_block;
    fsa->next_block = (size_t)block_pointer - (size_t)fsa;
}

size_t FSACountFree(const fsa_t *fsa)
{
    size_t count = 0;
    size_t runner = 0;

    assert (NULL != fsa);

    runner = fsa->next_block;

    while(0 != runner)
    {
        runner = *(size_t*)((char *)fsa + runner);
        ++count;
    }
    return count;
}

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
    size_t suggested_size = sizeof(fsa_t);

        if (0 != block_size % WORDSIZE)
    	{
    		block_size += WORDSIZE - (block_size % WORDSIZE);
    	}

        suggested_size += (block_size * num_of_blocks);

        return suggested_size;
}
