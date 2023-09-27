#include <stdio.h> /*print*/
#include <unistd.h>
#include <omp.h>


int main ()
{
    size_t i = 0;
    size_t sum = 0;
    const size_t div = 12345678912;

    omp_set_num_threads(12);

    //printf("before: %d\n", omp_get_num_threads());

    #pragma omp parallel
    {
        #pragma omp for
        for (i = 1; i <= div; ++i)
        {
            if (0 == div % i)
            {
                sum += i;
            }
        }
    }

    printf("sum = %ld\n", sum);
    return 0;
}
