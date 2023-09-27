#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX_WORDS (104334)
#define MAX_WORD_LENGTH (50)
#define COPIES (250)
#define MAX_COMBINED (COPIES * MAX_WORDS)
#define NUM_THREADS (10)
#define BILLION  1000000000L

void ReadToDic(void);

static void CopyArray(void);

static void ShuffleArrays(void);

static void CombineArrays(void);

static int StrCmp(const void *s1, const void *s2);

static int ShuffleCmp(const void *a, const void *b);

void *Organize(void *arr);

int MergeSort(int *arr_to_sort, size_t num_elements);

static int MergeArrays(int *arr1, size_t size1 ,int *arr2, size_t size2);

char dic[MAX_WORDS][MAX_WORD_LENGTH] = {0};
char cop_arr[COPIES][MAX_WORDS][MAX_WORD_LENGTH] = {0};
char comb_arr[MAX_COMBINED][MAX_WORD_LENGTH] = {0};

int main(void)
{
    size_t i = 0;
    pthread_t t_id[NUM_THREADS]= {0};
    struct timespec start, stop;
    double accum;

    if(clock_gettime(CLOCK_REALTIME, &start) == -1) 
    {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    ReadToDic();
    CopyArray();
    ShuffleArrays();
    CombineArrays();

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&t_id[i], NULL, Organize, comb_arr + (i * MAX_COMBINED / NUM_THREADS));
    }

    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(t_id[i], NULL);
    }

    //qsort(comb_arr, MAX_COMBINED, MAX_WORD_LENGTH, StrCmp);
    
    MergeSort(comb_arr, MAX_COMBINED);

    if( clock_gettime(CLOCK_REALTIME, &stop) == -1 ) {
      perror( "clock gettime" );
      return EXIT_FAILURE;
    }

    accum = ( stop.tv_sec - start.tv_sec )
             + (double)( stop.tv_nsec - start.tv_nsec )
               / (double)BILLION;

    printf("Threads: %d, time: %lf\n", NUM_THREADS, accum);

    return 0;
}

void ReadToDic(void)
{
	size_t i = 0;
	FILE *fp = NULL;

	fp = fopen("/usr/share/dict/american-english", "r");
	
	while (fgets(dic[i], MAX_WORD_LENGTH, fp))
	{
		dic[i][strcspn(dic[i], "\n")] = '\0';
        i++;
	}
    
    fclose(fp);
}

static void CopyArray(void)
{
    size_t i = 0;

    for (i = 0; i < COPIES; ++i)
    {
        memcpy(cop_arr[i], dic, sizeof(dic));
    }
}

static void ShuffleArrays(void)
{
    size_t i = 0;

    for (i = 0; i < COPIES; i++)
    {
        qsort(cop_arr[i], MAX_WORDS, MAX_WORD_LENGTH, ShuffleCmp);
    }
}

static void CombineArrays(void)
{
    size_t i = 0;

    for (i = 0; i < COPIES; i++)
    {
        memcpy(comb_arr + (i * MAX_WORDS), cop_arr[i], sizeof(dic));
    }
}

static int StrCmp(const void *s1, const void *s2)
{
    return strcmp((char *)s1, (char *)s2);
}

static int ShuffleCmp(const void *a, const void *b)
{
    (void)a;
    (void)b;

    return rand() - rand();
}

void *Organize(void *arr)
{
    qsort(arr, (MAX_COMBINED / NUM_THREADS), MAX_WORD_LENGTH, StrCmp);

    return arr;
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

static int MergeArrays(int *arr1, size_t size1 ,int *arr2, size_t size2)
{
    size_t i = 0, j = 0, k = 0, total_size = size1 + size2;
    int *merged_array = (int *)malloc(total_size * sizeof(int));

    if(NULL == merged_array)
    {
        return 1;
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

    return 0;
}