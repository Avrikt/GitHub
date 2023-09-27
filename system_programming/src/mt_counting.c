#define _POSIX_C_SOURCE 200809L
#include <stdio.h>      /*printf*/
#include <stdlib.h>     /*calloc()*/
#include <pthread.h>    /*pthread_create*/
#include <string.h>     /*memcpy*/
#include <time.h>       /*time functions*/

#include "../include/utilities.h"

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define ASCII (256)
#define NUM_THREADS (14)
#define CHARS_IN_DIC (880751)
#define COPIES (1000)
#define MAX_COMB (CHARS_IN_DIC * COPIES)
#define BILLION  1000000000L

static void TestCountingInts(void);

int CountingSortInts(int *array, size_t size, size_t num_of_threads);

int CountingSortChars(char *array, size_t size, size_t num_of_threads);

static void TestCountingChars(void);

void *ThreadInts(void *arg);

void *ThreadChars(void *arg);

void ReadToDic(void);

static void CopyArrays(void);

static int GetMax(int arr[], size_t size);

static int TestSortedArray(int array[], size_t size);

typedef struct thread
{
    int *array;
    int max_val;
    size_t start;
    size_t end;
    int *count_array;
}thread_args_t;

typedef struct ch_thread
{
    char *array;
    size_t start;
    size_t end;
}ch_thread_t;

pthread_mutex_t mutex;
int *glo_count_array = NULL;
int glo_char_lut[ASCII] = {0};

char dic[CHARS_IN_DIC] = {0};
char comb_arr[MAX_COMB] = {0};

int main(void)
{
    TestCountingInts();
    TestCountingChars();

    return 0;
}

static void TestCountingInts(void)
{
    int array[] = {5, 6, 2, 745, 85, 2, 26, 1, 26, 121, 8, 84, 6, 94, 6, 3, 121, 85};
    size_t size = sizeof(array)/ sizeof(array[0]);

    CountingSortInts(array, size, NUM_THREADS);

    HEADER(CountingSortInts);
    TEST(SUCCESS == TestSortedArray(array, size), CountingSortInts);
}

static void TestCountingChars(void)
{
    size_t i = 0;
    struct timespec start, stop;
    double accum;

    for(i = 1; i <= NUM_THREADS; ++i)
    {
        if(clock_gettime(CLOCK_REALTIME, &start) == -1) 
        {
        perror( "clock gettime" );
        return;
        }

        ReadToDic();
        CopyArrays();
        
        HEADER(TestCountingChars);
        TEST(SUCCESS == CountingSortChars(comb_arr, sizeof(comb_arr), NUM_THREADS), CountingSortChars);
        
        if( clock_gettime(CLOCK_REALTIME, &stop) == -1 ) {
        perror( "clock gettime" );
        return;
        }

        accum = ( stop.tv_sec - start.tv_sec )
                + (double)( stop.tv_nsec - start.tv_nsec )
                / (double)BILLION;

        printf("Threads: %ld, time: %f\n", i, accum);
    }
}

int CountingSortInts(int *array, size_t size, size_t num_of_threads)
{
    size_t i = 0, j = 0;
    pthread_t *th_id = (pthread_t *) malloc(num_of_threads * sizeof(pthread_t));
    thread_args_t *thread_data = (thread_args_t *)malloc(num_of_threads * sizeof(thread_args_t));
    size_t chunk_size = size / num_of_threads;
    int max_val = GetMax(array, size);
    glo_count_array = (int *)calloc((max_val + 1), sizeof(int));
    pthread_mutex_init(&mutex, NULL);

    if(th_id == NULL || thread_data == NULL || glo_count_array == NULL)
    {
        free(th_id);
        free(thread_data);
        free(glo_count_array);
        return FAILURE;
    }

    for(i = 0; i < num_of_threads; ++i)
    {
        thread_data[i].count_array = (int *)calloc(max_val + 1, sizeof(int));
        
        if(NULL == thread_data[i].count_array)
        {
            while(i--)
            {
                free(thread_data[i].count_array);
            }
            free(th_id);
            free(thread_data);
            free(glo_count_array);
            return FAILURE;
        }
        
        thread_data[i].array = array;
        thread_data[i].start = (i * chunk_size);
        thread_data[i].end = (i + 1) * chunk_size;
        thread_data[i].max_val = max_val;
        if(i == num_of_threads - 1)
        {
            thread_data[i].end += size % num_of_threads;
        }

        pthread_create(&th_id[i], NULL, ThreadInts, &thread_data[i]);
    }

    for (i = 0; i < num_of_threads; ++i)
    {
        pthread_join(th_id[i], NULL);
    }

    for (i = 0; i <= (size_t)max_val; ++i)
    {
        while(0 < glo_count_array[i]--)
        {
            array[j++] = i;
        }
    }

    for(i = 0;i < num_of_threads; ++i)
    {
        free(thread_data[i].count_array);
    }

    free(th_id);
    free(thread_data);
    free(glo_count_array);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *ThreadInts(void *arg)
{
    thread_args_t *data = (thread_args_t *)arg;
    size_t i = 0;

    for(i= data->start; i < (size_t)data->end; ++i)
    {
        data->count_array[data->array[i]]++;
    }
    
    pthread_mutex_lock(&mutex);
    for (i = 0;i <= (size_t)data->max_val; ++i)
    {
        glo_count_array[i] += data->count_array[i];
    }

    pthread_mutex_unlock(&mutex);
    

    return NULL;
}

int CountingSortChars(char *array, size_t size, size_t num_of_threads)
{
    size_t i = 0, j = 0;
    pthread_t *th_id = (pthread_t *) malloc(num_of_threads * sizeof(pthread_t));
    ch_thread_t *ch_thread_data = (ch_thread_t *)malloc(num_of_threads * sizeof(ch_thread_t));
    size_t chunk_size = size / num_of_threads;
    pthread_mutex_init(&mutex, NULL);

    if(th_id == NULL || ch_thread_data == NULL)
    {
        free(th_id);
        free(ch_thread_data);
        return FAILURE;
    }

    for(i = 0; i < num_of_threads; ++i)
    {
        ch_thread_data[i].array = array;
        ch_thread_data[i].start = (i * chunk_size);
        ch_thread_data[i].end = (i + 1) * chunk_size;
        if(i == num_of_threads - 1)
        {
            ch_thread_data[i].end = size;
        }

        pthread_create(&th_id[i], NULL, ThreadChars, &ch_thread_data[i]);
    }

    for (i = 0; i < num_of_threads; ++i)
    {
        pthread_join(th_id[i], NULL);
    }
    
    for (i = 1; i < ASCII; ++i)
    {
        while(0 < glo_char_lut[i]--)
        {
            array[j++] = (char)i;
        }
    }

    free(th_id);
    free(ch_thread_data);
    pthread_mutex_destroy(&mutex);

    return SUCCESS;
}

void *ThreadChars(void *arg)
{
    ch_thread_t *data = (ch_thread_t *)arg;
    size_t i = 0;
    int lut[ASCII]= {0};

    for(i = data->start; i < data->end; ++i)
    {
        ++lut[(int)data->array[i]];
    }

    pthread_mutex_lock(&mutex);
    for (i = 0;i < ASCII; ++i)
    {
        glo_char_lut[i] += lut[i];
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void ReadToDic(void)
{
	size_t i = 0;
	FILE *fp = NULL;
    char ch = 0;

	fp = fopen("/usr/share/dict/american-english", "r");
	
    while(!feof(fp))
    {
        ch = fgetc(fp);
        if('\n' != ch && '\0' != ch)
        {
           dic[i] = ch;
        }
        ++i;
    }
    
    fclose(fp);
}

static void CopyArrays(void)
{
    size_t i = 0;

    for (i = 0; i < COPIES; i++)
    {
        memcpy(comb_arr + (i * CHARS_IN_DIC), dic, sizeof(dic));
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

static int TestSortedArray(int array[], size_t size)
{
	size_t i = 0;

	for (i = 0; i < size - 1; ++i)
	{
		printf("%d->", array[i]);
        if(array[i] > array[i + 1])
		{
			return 1;
		}
	}
    printf("%d\n", array[i]);

	return 0;
}