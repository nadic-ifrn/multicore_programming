//cálculo de uma aproximação do número Pi através da soma de uma série infinita (série de Leibniz). 
#include <omp.h>
#include <stdio.h>

#define N 10000000
#define d 1.0

int main() {
    int i;
    double pi = 0.0;

    double start = omp_get_wtime();

    // Com schedule(static, chunk)
    #pragma omp parallel for schedule(static, 100)
    for(i = 0; i < N; i++) {
        double x = (i % 2 == 0 ? 1 : -1) * 4.0 / (2.0 * i + 1);
        #pragma omp atomic
        pi += x;
    }

    double end = omp_get_wtime();

    printf("PI = %.16lf\n", pi);
    printf("Tempo com schedule(static, 100): %lf segundos\n", end - start);

    pi = 0.0;
    start = omp_get_wtime();

    // Com schedule(dynamic, chunk)
    #pragma omp parallel for schedule(dynamic, 100)
    for(i = 0; i < N; i++) {
        double x = (i % 2 == 0 ? 1 : -1) * 4.0 / (2.0 * i + 1);
        #pragma omp atomic
        pi += x;
    }

    end = omp_get_wtime();

    printf("PI = %.16lf\n", pi);
    printf("Tempo com schedule(dynamic, 100): %lf segundos\n", end - start);

    pi = 0.0;
    start = omp_get_wtime();

    // Com schedule(guided, chunk)
    #pragma omp parallel for schedule(guided, 100)
    for(i = 0; i < N; i++) {
        double x = (i % 2 == 0 ? 1 : -1) * 4.0 / (2.0 * i + 1);
        #pragma omp atomic
        pi += x;
    }

    end = omp_get_wtime();

    printf("PI = %.16lf\n", pi);
    printf("Tempo com schedule(guided, 100): %lf segundos\n", end - start);

    return 0;
}
