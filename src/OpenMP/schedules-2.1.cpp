#include <omp.h>
#include <stdio.h>

int main() {
    const int N = 16;
    int i;

    printf("Schedule static:\n");
    #pragma omp parallel for schedule(static) num_threads(4)
    for(i = 0; i < N; i++) {
        printf("Thread %d processou a iteração %d\n", omp_get_thread_num(), i);
    }

    printf("\nSchedule dynamic:\n");
    #pragma omp parallel for schedule(dynamic,10) num_threads(4)
    for(i = 0; i < N; i++) {
        printf("Thread %d processou a iteração %d\n", omp_get_thread_num(), i);
    }

    printf("\nSchedule guided:\n");
    #pragma omp parallel for schedule(guided) num_threads(4)
    for(i = 0; i < N; i++) {
        printf("Thread %d processou a iteração %d\n", omp_get_thread_num(), i);
    }

    return 0;
}
