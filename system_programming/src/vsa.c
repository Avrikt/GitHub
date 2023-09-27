/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Omri Shalev					*
* Date: 12.03.23							*
* Description: Source file for vsa		    *
*********************************************/

/************************************LIBRARIES**************************************************/

#include <assert.h> /*assert*/

#include "vsa.h"

/***************************PROTOTYPES AND GLOBAL VARIABLES**************************************/
#define WORDSIZE sizeof(size_t)
#define FREE (1)

static void VSADefrag(vsa_t *pool);

struct vsa
{
    size_t size;
};
/************************************Functions***************************************************/

vsa_t *VSAInit(void *memory_pool, size_t segment_size)
{
    vsa_t *vsa = NULL;
    vsa_t *runner = NULL;
    size_t remain = (size_t)memory_pool % WORDSIZE;

    assert (NULL != memory_pool);
    assert (0 != segment_size);

    /*align address*/
    if (0 != remain)
    {
            memory_pool = (size_t *)((size_t)memory_pool + WORDSIZE - remain);
            segment_size -= remain;
    }

    /*align segment to word size*/
    remain = segment_size % WORDSIZE;

    if (0 != remain)
    {
        segment_size -= remain;
    }

    vsa = (vsa_t *)memory_pool;
    vsa->size = segment_size;

    runner = (vsa_t *)((char *)vsa + WORDSIZE);
    runner->size = (segment_size - WORDSIZE) + FREE;

    return vsa;
}

void *VSAAlloc(vsa_t *pool, size_t block_size)
{
    vsa_t *runner = (vsa_t *)((char *)pool + WORDSIZE);
    void *pointer = NULL;
    size_t tmp_size = 0;
    size_t remain = block_size % WORDSIZE;

    assert(NULL != pool);

    if (0 != remain)
    {
        block_size += WORDSIZE - remain;
    }

    if (block_size > VSALargestChunkAvailable(pool))
    {
        return NULL;
    }

    while ((FREE != (runner->size & FREE)) || (runner->size < block_size))
    {
        runner = (vsa_t*)((char *)runner + (runner->size));
    }

    tmp_size = runner->size - FREE;
    runner->size = block_size + WORDSIZE;
    pointer = (vsa_t *)((char *)runner + WORDSIZE);
    if ((char*)pool + pool->size != (char *)runner + runner->size)
    {
        runner = (vsa_t*)((char *)runner + runner->size);
        runner->size = (tmp_size - block_size - WORDSIZE) + FREE;
    }
    return pointer;
}

void VSAFree(void *block_pointer)
{
    vsa_t *block = NULL;
    assert (NULL != block_pointer);

    block = (vsa_t *)((char*)block_pointer - WORDSIZE);
    block->size = block->size + FREE;
}

size_t VSALargestChunkAvailable(vsa_t *pool)
{
    vsa_t *runner = pool + 1;
    char *end_of_block = (char*)pool + pool->size;
    size_t largest_chunk = 0;

    assert (NULL != pool);

    VSADefrag(pool);

    while (end_of_block > (char*)runner)
    {
        if (FREE == (runner->size & FREE))
        {
            if (runner->size > largest_chunk)
            {
                largest_chunk = runner->size - FREE;
            }
        }
        runner = (vsa_t*)((char *)runner + (runner->size & (~FREE)));
    }

    return (WORDSIZE == largest_chunk) ? 0 : largest_chunk - WORDSIZE;
}

static void VSADefrag(vsa_t *pool)
{
    vsa_t *runner = (vsa_t *)((char *)pool + WORDSIZE);
    vsa_t *free_run = runner;
    char *end_of_block = (char*)pool + pool->size;

    assert (NULL != pool);

    while (end_of_block > (char *)runner)
    {
        if (FREE == (runner->size & FREE))
        {
            free_run = (vsa_t*)((char *)runner + ((runner->size - 1)));

            while (end_of_block != (char *)free_run && FREE == (free_run->size & FREE))
            {
                runner->size += --free_run->size;
                free_run = (vsa_t*)((char *)free_run + (free_run->size));
            }
        }
        runner = (vsa_t*)((char *)runner + (runner->size));
    }
}
