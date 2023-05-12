//clang -fopenmp hello.c -o bin/hello
//./bin/hello
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num_threads = 6;
    omp_set_num_threads(num_threads);
    printf("OpenMP version %d.%d\n", _OPENMP / 100, _OPENMP % 100);
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello, world! This is thread %d out of %d.\n", id, num_threads);
    }

    return 0;
}