/********************************************
* Developer: Avri Kehat						*
* Reviewed by: Adir Ortal					*
* Date: 28.03.23							*
* Description: Source file for sort			*
*********************************************/

/************************************LIBRARIES**************************************************/
#include <assert.h> /*assert*/
#include <stdlib.h> /*calloc, free*/

#include "sort.h"
#include "utilities.h"

/**************************MACROS AND FORWARD DECLERATIONS**************************************/
#define DECIMAL (10)

static int CountingSortForRadix(int array[], size_t size, size_t place);
static int MergeArrays(int *arr1, size_t size1 ,int *arr2, size_t size2);
static void Swap(int *ptr1, int *ptr2);
static void VoidSwap(void *ptr1, void *ptr2, size_t size);
static int GetMax(int arr[], size_t size);
static int GetMin(int arr[], size_t size);

/************************************Functions***************************************************/

void BubbleSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0;
    int swap_flag = 0;

    assert(NULL != cmp_func);

    do
    {
        swap_flag = 0;
        for (i = 0; i < size - 1; ++i)
        {
            if(0 < cmp_func(array[i], array[i + 1]))
            {
                Swap(array + i, array + i + 1);
                swap_flag = 1;
            }
        }
    } while(0 != swap_flag);
}

void InsertionSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0, j = 0;

    assert(NULL != cmp_func);

    for (i = 1; i < size; ++i)
    {
        for (j = i;0 < j && 0 < cmp_func(array[j - 1],array[j]); --j)
        {
            Swap(array + j, array + j - 1);
        }
    }
}

void SelectionSort(int array[], size_t size, int (*cmp_func)(const int lhs, const int rhs))
{
    size_t i = 0, j = 0, min_val = 0;

    assert(NULL != cmp_func);

    for (i = 0; i < size - 1; ++i)
    {
        min_val = i;
        for (j = i; j < size; ++j)
        {
            if(0 < cmp_func(array[min_val], array[j]))
            {
                min_val = j;
            }
        }

        if (min_val != i)
        {
            Swap(array + i ,array + min_val);
        }
    }
}

int CountingSort(int array[], size_t size)
{
    int *indx_array = NULL;
    size_t i = 0, j = 0;
    int max_val = GetMax(array, size);
    int min_val = GetMin(array, size);

    indx_array =(int *)calloc((max_val - min_val + 1), sizeof(int));
    if (NULL == indx_array)
    {
        return FAILURE;
    }

    for (i = 0; i < size; ++i)
    {
        ++indx_array[array[i] - min_val];
    }

    for (i = min_val; i <= (size_t)max_val; ++i)
    {
        while (0 < indx_array[i]--)
        {
            array[j++] = i;
        }
    }
    free(indx_array);

    return 0;
}

int RadixSort(int array[], size_t size)
{
    size_t place = 1;
    int max_val = GetMax(array, size);

    for (place = 1; max_val / place > 0; place *= 10)
    {
        if (SUCCESS != CountingSortForRadix(array, size, place))
        {
            return FAILURE;
        }
    }
    return SUCCESS;
}

ssize_t BinarySearchIterative(int *arr, size_t size, int num)
{
    size_t begin = 0, end = 0;
    ssize_t median = 0;

    assert(NULL != arr);

    end = size - 1;

    while (begin <= end && (num != *(arr + median)))
    {
        median = (begin + end) / 2;
        if (num > *(arr + median))
        {
            begin = median + 1;
        }
        else
        {
            end = median;
        }
    }

    return (num == *(arr + median)) ? median : -1;
}

ssize_t BinarySearchRecursive(int *arr, size_t size, int num)
{
    ssize_t median = (size / 2);
    int res = 0;

    if (num == *(arr + median))
    {
        return median;
    }

    if (1 >= size)
    {
        return -1;
    }

    if (num < *(arr + median))
    {
        return BinarySearchRecursive(arr, median, num);
    }
    else
    {
        res = BinarySearchRecursive(arr + median, median + 1, num);

        return (-1 == res) ? res : res + median;
    }
}

int MergeSort(int *arr_to_sort, size_t num_elements)
{
    size_t median = num_elements / 2;

    if (1 >= num_elements)
    {
        return 0;
    }

    MergeSort(arr_to_sort, median);
    MergeSort(arr_to_sort + median, num_elements - median);
    return MergeArrays(arr_to_sort, median, arr_to_sort + median, num_elements - median);
}

void QSort(void *base, size_t num_elements, size_t size, int (*compar)(const void *, const void *))
{
    size_t arr_size = (num_elements - 1) * size;
    char *pivot = NULL;
    char *bigger = NULL;
    char *smaller = NULL;

    assert(NULL != base);
    assert(NULL != compar);

    if(1 >= num_elements)
    {
        return;
    }

    pivot = (char *)base + arr_size;
    bigger = (char *)base;
    smaller = (char *)base + arr_size;

    while(bigger <= smaller)
    {
        while(0 < compar(pivot, bigger))
        {
            bigger += size;
        }

        while(0 > compar(pivot, smaller))
        {
            smaller = (char *)smaller - size;
        }

        if(bigger <= smaller)
        {
            VoidSwap(bigger, smaller, size);
            bigger += size;
            smaller -= size;
        }
    }

    QSort(base, ((smaller - (char *)base) / size) + 1, size, compar);
    QSort(bigger, ((char *)base + num_elements * size - bigger)/ size, size, compar);
}

/**********************************Static Functions********************************************/
static void Swap(int *ptr1, int *ptr2)
{
    int tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

static void VoidSwap(void *ptr1, void *ptr2, size_t size)
{
    char tmp = 0;
    char *run1 = (char *)ptr1;
    char *run2 = (char *)ptr2;

    while(0 < size--)
    {
        tmp = *run1;
        *run1++ = *run2;
        *run2++ = tmp;
    }
}

static int GetMax(int arr[], size_t size)
{
    size_t i = 0;
    int max = arr[0];

    for (i = 0; i < size; ++i)
    {
        max = MAX(max, arr[i]);
    }

    return max;
}

static int GetMin(int arr[], size_t size)
{
    size_t i = 0;
    int min = arr[0];

    for (i = 0; i < size; ++i)
    {
        min = MIN(min, arr[i]);
    }

    return min;
}

static int CountingSortForRadix(int array[], size_t size, size_t place)
{
    int indx_array[DECIMAL] = {0};
    int *output_array = NULL;
    size_t i = 0;

    output_array = (int *) calloc(size, sizeof(int));
    if(NULL == output_array)
    {
        return FAILURE;
    }

    for (i = 0; i < size; ++i)
    {
        ++indx_array[(array[i]/place) % DECIMAL];
    }

    for (i = 1; i < DECIMAL; ++i)
    {
        indx_array[i] += indx_array[i - 1];
    }

    for (i = size - 1; (int)i >= 0; --i)
    {
        output_array[--indx_array[(array[i]/place) % DECIMAL]] = array[i];
    }

    for (i = 0; i < size; ++i)
    {
        array[i] = output_array[i];
    }

    free(output_array);

    return 0;
}

static int MergeArrays(int *arr1, size_t size1 ,int *arr2, size_t size2)
{
    size_t i = 0, j = 0, k = 0, total_size = size1 + size2;
    int *merged_array = (int *)malloc(total_size * sizeof(int));

    if(NULL == merged_array)
    {
        return FAILURE;
    }

    for(i = 0; i < total_size; ++i)
    {
        if(k >= size2 || (j < size1 && arr1[j] < arr2[k]))
        {
            merged_array[i] = arr1[j++];
        }
        else
        {
            merged_array[i] = arr2[k++];
        }
    }

    for(i = 0; i < total_size; ++i)
    {
        *(arr1 + i) = merged_array[i];
    }

    free(merged_array);

    return SUCCESS;
}
