#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double calculate_pi(unsigned long n)
{
    unsigned long i, h = 0;
    double x, y;
    
    double start, end;
    start = omp_get_wtime();

    #pragma omp parallel
    {
        unsigned int seed = clock() * omp_get_thread_num();
        #pragma omp for reduction(+ : h) private(x,y)
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
    return 4.0 * h / n;
}

int main(int argc, char **argv)
{
    unsigned long n = strtol(argv[1], NULL, 10);
    double pi = calculate_pi(n);
    printf("Pi: %1.15f\n", pi);
}
