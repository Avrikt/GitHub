/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Mor Zibel					*
* Date: 15.03.23							*
* Description: Source file for Recursion	*
*********************************************/

/************************************LIBRARIES**************************************************/

#include <assert.h> /*assert*/
#include <string.h> /*strncmp*/

#include "recursion.h"
#include "stack.h"

/********************************FORWARD DECLERATIONS*******************************************/

static void StackPushRec(stack_t *stack, void *data);
static void *StackPeekPop(stack_t *stack);

/************************************Functions***************************************************/
unsigned long FibonacciIter(size_t element_index)
{
    unsigned long first = 0, second = 1, tmp = 0;
    size_t i = 0;

    for (i = 0; i < element_index; ++i)
    {
        tmp = second;
        second += first;
        first = tmp;
    }

    return first;
}

unsigned long FibonacciRec(size_t element_index)
{
    if (1 >= element_index)
    {
        return element_index;
    }

    return FibonacciRec(element_index - 1) + FibonacciRec(element_index - 2);
}

node_t *FlipList(node_t *node)
{
    node_t *new_head = NULL;

    if (NULL == node || NULL == node->next)
    {
        return node;
    }

    new_head = FlipList(node->next);

    node->next->next = node;
    node->next = NULL;

    return new_head;
}


size_t StrLenRec(const char *s)
{
    assert (NULL != s);

    if ('\0' == *s)
    {
        return 0;
    }

    return StrLenRec(s + 1) + 1;
}

int StrCmpRec(const char *s1, const char *s2)
{
    assert (NULL != s1);
    assert (NULL != s2);

    if (*s1 != *s2 || '\0' == *s1 || '\0' == *s2)
    {
        return *s1 - *s2;
    }

    return StrCmpRec(s1 + 1, s2 + 1);
}

char *StrCpyRec(char *dest, const char *src)
{
    assert (NULL != dest);
    assert (NULL != src);

    if ('\0' == *src)
    {
        *dest = *src;
        return dest;
    }

    *dest = *src;
    return StrCpyRec(dest + 1, src + 1) - 1;
}

char *StrCatRec(char *dest, const char *src)
{
    assert (NULL != dest);
    assert (NULL != src);

    if ('\0' == *dest)
    {
        StrCpyRec(dest, src);
        return dest;
    }

    return StrCatRec(dest + 1, src) - 1;
}

char *StrStrRec(const char *haystack, const char *needle)
{
    size_t length = StrLenRec(needle);

    assert (NULL != haystack);
    assert (NULL != needle);

    if ('\0' == *haystack)
    {
        return NULL;
    }

    if (0 == strncmp(haystack, needle, length))
    {
        return (char *)haystack;
    }

    return StrStrRec(haystack + 1, needle);
}

void SortStack(stack_t *stack)
{
    void *data = NULL;

    assert (NULL != stack);

    if(1 == StackIsEmpty(stack))
    {
        return;
    }
    else
    {
        data = StackPeekPop(stack);
        SortStack(stack);
        StackPushRec(stack, data);
    }
}

static void StackPushRec(stack_t *stack, void *data)
{
    int num = 0;

    assert (NULL != stack);

    if ((1 == StackIsEmpty(stack)) || (*(int *)StackPeek(stack) < *(int *)data))
    {
        StackPush(stack, data);
    }
    else
    {
        num = *(int *)StackPeekPop(stack);
        StackPushRec(stack, data);
        StackPush(stack, &num);
    }
}

static void *StackPeekPop(stack_t *stack)
{
    void *data = NULL;

    assert(NULL != stack);

    data = StackPeek(stack);
    StackPop(stack);

    return data;
}
