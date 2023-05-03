//gcc -fopenmp hello.c -o hello
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num_threads = 6;
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello, world! This is thread %d out of %d.\n", id, num_threads);
    }

    return 0;
}