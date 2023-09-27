/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Tomer Savorai				*
* Date: 16.04.23							*
* Description: Source file for Hash Table	*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/
#include <math.h>   /*pow, sqrt*/

#include "hash.h"
#include "dll.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/

struct hash_table
{
    dll_t **hash_table_array;
    size_t capacity;
    size_t (*hash_func)(const void *value);
    int (*is_match_func)(const void *lhd, const void *rhd);
};

static int TableCounter(void *dummy, void *count);

/************************************Functions***************************************************/

hash_table_t *HashTableCreate(size_t capacity, size_t(*hash_func)(const void *value),
                    int(*is_match_func)(const void *lhd, const void *rhd))
{
    hash_table_t *new_table = NULL;
    size_t i = 0;

    assert(NULL != hash_func);
    assert(NULL != is_match_func);

    new_table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if(NULL == new_table)
    {
        return NULL;
    }

    new_table->capacity = capacity;
    new_table->hash_func = hash_func;
    new_table->is_match_func = is_match_func;

    new_table->hash_table_array = (dll_t **) calloc(capacity, sizeof(dll_t *));
    if(NULL == new_table->hash_table_array)
    {
        free(new_table);
        return NULL;
    }

    for (i = 0; i < capacity; ++i)
    {
        new_table->hash_table_array[i] = DLLCreate();
        if (NULL == new_table->hash_table_array[i])
        {
            HashTableDestroy(new_table);
            new_table = NULL;
        }
    }

    return new_table;
}  

void HashTableDestroy(hash_table_t *table)
{
    size_t i = 0;

    for(i = 0; i < table->capacity; ++i)
    {
        DLLDestroy(table->hash_table_array[i]);
    }

    free(table->hash_table_array);
    free(table);
}

int HashTableInsert(hash_table_t *table, void *data)
{
    size_t index = 0;
    dll_t *bucket = NULL;

    assert(NULL != table);

    index = table->hash_func(data) % table->capacity;
    bucket = table->hash_table_array[index];

    return !(FALSE == DLLIsSameIter(DLLInsert(DLLEndIter(bucket), data), DLLEndIter(bucket)));
}

void HashTableRemove(hash_table_t *table, const void *data)
{
    size_t index = 0;
    dll_t *bucket = NULL;

    assert(NULL != table);

    index = table->hash_func(data) % table->capacity;
    bucket = table->hash_table_array[index];

    DLLRemove(DLLFind(DLLBeginIter(bucket), DLLEndIter(bucket), table->is_match_func, data));
} 

size_t HashTableSize(const hash_table_t *table)
{
    size_t size = 0;

    assert(NULL != table);

    HashTableForEach(table, TableCounter, &size);

    return size;
}

int HashTableIsEmpty(const hash_table_t *table)
{
    assert(NULL != table); 
    
    return (0 == HashTableSize(table));
}

void *HashTableFind(const hash_table_t *table, const void *data)
{
    size_t index = 0;
    dll_t *bucket = NULL;
    iterator_t to_find = NULL;
    void *return_data = NULL;

    assert(NULL != table);

    index = table->hash_func(data) % table->capacity;
    bucket = table->hash_table_array[index];

    to_find = DLLFind(DLLBeginIter(bucket), DLLEndIter(bucket), table->is_match_func, data);

    if (TRUE != DLLIsSameIter(to_find, DLLEndIter(bucket)))
    {
        return_data = DLLGetData(to_find);
        DLLRemove(to_find);
        DLLPushFront(bucket, return_data);
    }

    return return_data;
}   

int HashTableForEach(const hash_table_t *table, int (*action_func)(void *data, void *param), void *param)
{
    dll_t * current = NULL;
    int success_flag = SUCCESS;
    size_t i = 0;

    for (i = 0; i < table->capacity && SUCCESS == success_flag; ++i)
    {
        current = table->hash_table_array[i];
        success_flag = DLLForEach(DLLBeginIter(current), DLLEndIter(current), action_func, param);
    }

    return success_flag;
}
  
double HashLoad(const hash_table_t *table)
{
    assert(NULL != table);

    return (double) HashTableSize(table) / (double)table->capacity;
}

double HashSTDev(const hash_table_t *table)
{
    size_t i = 0;
    double mean = HashLoad(table);
    double size = (double)HashTableSize(table);
    double sum = 0;
    double deviation = 0;

    for (i = 0; i < table->capacity; ++i)
    {
        deviation = DLLCount(table->hash_table_array[i]) - mean;
        sum += pow(deviation, 2)/size; 
    }

    return sqrt(sum);
}

/************************************ Static Functions *******************************************/
static int TableCounter(void *dummy, void *count)
{
	++*(size_t *)count;
	(void) dummy;
	return 0;
}