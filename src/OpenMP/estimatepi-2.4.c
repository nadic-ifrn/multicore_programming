//clang estimatepi-2.4.c -fopenmp  -o bin/estimatepi-2.4
//./bin/estimatepi-2.4 100000000

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    unsigned long i, h = 0, n = strtol(argv[1], NULL, 10);
    double x, y;
    
    double start, end;
    start = omp_get_wtime();
#pragma omp parallel
    {
        unsigned int seed = clock() * omp_get_thread_num();
#pragma omp for reduction(+ : h)
        for (i = 0; i < n; i++)
        {
            x = ((double)rand_r(&seed)) / RAND_MAX;
            y = ((double)rand_r(&seed)) / RAND_MAX;
            if (x * x + y * y <= 1.0)
                h++;
        }
    }
    end = omp_get_wtime();
    printf("Tempo decorrido = %.16g segundos\n", end - start);
    printf("Pi: %1.15f\n", 4.0 * h / n);
}