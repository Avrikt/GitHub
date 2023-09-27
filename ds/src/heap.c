/********************************************
* Developer: Avri Kehat						*
* Reviewed by:								*
* Date: xx.xx.23							*
* Description: Source file for Heap			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <stdlib.h> /*malloc, free*/
#include <assert.h> /*assert*/

#include "heap.h"
#include "d_vector.h"
#include "utilities.h"

/********************************FORWARD DECLERATIONS*******************************************/
#define ROOT (0)
#define PARENT(index) (((index) - 1) / 2) 
#define LEFT_CHILD(index) (2 * (index) + 1) 
#define RIGHT_CHILD(index) (2 * (index) + 2) 
#define LAST_INDX(heap) (HeapSize(heap) - 1)
#define VECTOR_SIZE (3)

static void HeapifyUp(heap_t *heap, size_t current);

static void HeapifyDown(heap_t *heap, size_t current);

static void *RemoveByIndex(heap_t *heap, size_t indx);

static void SwapValue(void **a, void **b);
struct heap 
{
	vector_t *vec;
    int (*cmp_func)(const void *, const void *);
};

/************************************Functions***************************************************/

heap_t *HeapCreate(int (*cmp_func)(const void *, const void *))
{
    heap_t *new_heap = NULL;
    
    assert(NULL != cmp_func);

    new_heap = (heap_t *)malloc(sizeof(heap_t));
    if (NULL == new_heap)
    {
        return NULL;
    }

    new_heap->vec = VectorCreate(VECTOR_SIZE, sizeof(void *));
    if (NULL == new_heap->vec)
    {
        free(new_heap);
        return NULL;
    }
    
    new_heap->cmp_func = cmp_func;
    
    return new_heap;    
}	

void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    
    VectorDestroy(heap->vec);
    free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
    int success_flag = SUCCESS;
    
    success_flag = VectorPushBack(heap->vec, (void *)&data);
    
    HeapifyUp(heap, LAST_INDX(heap));
    
    return success_flag;
}

void HeapPop(heap_t *heap)
{
    assert(NULL != heap);

    RemoveByIndex(heap, ROOT);
}

void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    
    return *(void **)VectorGetAccessToElement(heap->vec, ROOT);
}

size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    
    return VectorSize(heap->vec);
}

int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);

    return VectorIsEmpty(heap->vec);
}
							
void *HeapRemoveByKey(heap_t *heap, void *data)
{
    void *ret_data = NULL;
    size_t i = 0;
    size_t heap_size = 0;
    int cmp_result = 1;

    assert(NULL != heap);
    
    heap_size = HeapSize(heap);

    for (i = 0; i < heap_size && 0 != cmp_result; ++i)
    {
        ret_data = *(void **)VectorGetAccessToElement(heap->vec, i);
        cmp_result = heap->cmp_func(ret_data, data);
    }

    return (cmp_result == 0) ? RemoveByIndex(heap, --i) : NULL; 
}
                                       
void *HeapRemove(heap_t *heap, int (*is_match_func)(const void *data, const void *param), const void *param)
{
    void *data = NULL;
    size_t i = 0;
    size_t heap_size = 0;
    int match_flag = FALSE;

    assert(NULL != heap);
    assert(NULL != is_match_func);

    heap_size = HeapSize(heap);

    for (i = 0; i < heap_size && TRUE != match_flag; ++i)
    {
        data = *(void **)VectorGetAccessToElement(heap->vec, i);
        match_flag = is_match_func(data, param);
    }
    
    return (TRUE == match_flag) ? RemoveByIndex(heap, --i) : NULL;
}

/************************************ Static Functions *******************************************/
static void HeapifyUp(heap_t *heap, size_t current)
{
    void *val_current = VectorGetAccessToElement(heap->vec, current);
    void *val_parent = NULL;
    
    if (ROOT != current)
    {
        val_parent = VectorGetAccessToElement(heap->vec, PARENT(current));
    }

    if(ROOT == current || 0 < heap->cmp_func(*(void **)val_parent, *(void **)val_current))
    {
        return;
    }

    SwapValue(val_parent, val_current);

    HeapifyUp(heap, PARENT(current));

}

static void HeapifyDown(heap_t *heap, size_t current)
{
    void *right_child = VectorGetAccessToElement(heap->vec, RIGHT_CHILD(current));
    void *left_child = VectorGetAccessToElement(heap->vec, LEFT_CHILD(current));
    void *parent = VectorGetAccessToElement(heap->vec, current);
    void *bigger_child = NULL;
    size_t child_indx = 0;
    size_t last_indx = LAST_INDX(heap);
    
    if(current >= last_indx || LEFT_CHILD(current) > last_indx)
    {
        return;
    }

    if(RIGHT_CHILD(current) <= last_indx && 0 < heap->cmp_func(*(void **)right_child, *(void **)left_child))
    {
        bigger_child = right_child;
        child_indx = RIGHT_CHILD(current);
    }
    else
    {
        bigger_child = left_child;
        child_indx = LEFT_CHILD(current);
    }
        
    if(0 < heap->cmp_func(*(void **)bigger_child, *(void **)parent))
    {
        SwapValue(bigger_child, parent);
    }

    HeapifyDown(heap, child_indx);
}

static void *RemoveByIndex(heap_t *heap, size_t indx)
{
    size_t last_indx = LAST_INDX(heap);
    void *current_data = VectorGetAccessToElement(heap->vec, indx);
    void *last_data = VectorGetAccessToElement(heap->vec, last_indx);

    SwapValue(current_data, last_data);
    VectorPopBack(heap->vec);
    if (!HeapIsEmpty(heap))
    {
        HeapifyDown(heap, indx);
    }
    
    HeapifyUp(heap, indx);
    
    return *(void **)last_data;
}

static void SwapValue(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}