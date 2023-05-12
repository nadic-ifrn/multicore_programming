//clang estimatepi-1.0.c -fopenmp -o bin/estimatepi-1.0
//./bin/estimatepi-1.0 100000000

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
    unsigned long i, h = 0, n = strtol(argv[1], NULL, 10);
    double x, y;
    srand(clock());
   
    double start, end;
    start = omp_get_wtime();

    for (i = 0; i < n; i++)
    {
        x = ((double)rand()) / RAND_MAX;
        y = ((double)rand()) / RAND_MAX;
        if (sqrt(x * x + y * y) <= 1.0)
            h++;
    }
    end = omp_get_wtime();
    printf("Tempo decorrido = %.16g segundos\n", end - start);

    printf("Pi: %1.15f\n", 4.0 * h / n);
}