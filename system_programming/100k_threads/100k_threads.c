#include <pthread.h> /*threads*/
#include <stdio.h> /*prinf*/
#include <unistd.h> /*sleep*/
#include <time.h>

#define K (10000000000)
#define NUM_OF_T (12)

static size_t Load(size_t num);
void *Ex4(void *arg);

typedef struct index index_t;

struct index
{
    size_t start;
    size_t end;
};


int main(void)
{
    size_t jump = K / NUM_OF_T;
    index_t index = {1};
    size_t i = 0;
    pthread_t id[NUM_OF_T];
    int sum_of_div = 0;
    clock_t start_time = clock(), end_time;
    time_t sec = time(NULL);
    int *sum_p = &sum_of_div;

    index.end = jump;

    for (i = 0; i < NUM_OF_T; ++i)
    { 
        while (0 != pthread_create(&id[i], NULL, Ex4, &index))
        {

        }
        index.start = index.end + 1;
        index.end += jump;
    }

    i = 0;

    while(index.start < K)
    {
        pthread_join(id[i % NUM_OF_T ],(void *)sum_p);
        ++i;
        while( 0 != pthread_create(&id[i % NUM_OF_T], NULL, Ex4, &index) )
        {
                /*void*/
        }

        index.start = index.end + 1;
        index.end += jump;

        if (index.end > NUM_OF_T)
        {
            index.end = NUM_OF_T;
        }
    }

    end_time = clock();
    printf("Clock Time Loop with %d threads = %f \n", NUM_OF_T, (double)(end_time - start_time)/ CLOCKS_PER_SEC);
    printf("Time() = %ld \n", time(NULL) - sec);
    return 0;
}

void *Ex4(void *arg)
{
    index_t *index = (index_t *)arg;
    size_t i = index->start;
    int sum_of_div = 0;

    #pragma omp parallel num_threads(NUM_OF_T)
    for ( ;i < index->end; ++i)
    {
        if (0 == index->end % i)
        {
            sum_of_div += i;
        }
    }

    return NULL;
}

static size_t Load(size_t num)
{
    size_t i = 0;
    int sum_of_div = 0;

    for (i = 1; i < num; ++i)
    {
        if (0 == num % i)
        {
            sum_of_div += i;
        }
    }

    return sum_of_div;
}