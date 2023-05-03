#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
// #include "pascalops.h"

int main(int argc, char **argv)
{
    int seed = clock(), n = strtol(argv[1], NULL, 10);
    float(*A)[n] = malloc(n * sizeof *A);
    float(*L)[n] = malloc(n * sizeof *L);
    float(*U)[n] = malloc(n * sizeof *U);
    float *B = malloc (n * sizeof (float));
    float *X = malloc (n * sizeof (float));
    float *Y = malloc (n * sizeof (float));
#pragma omp parallel
    {
        // pascal_start(1);
#pragma omp for
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                A[i][j] = ((float)rand_r(&seed) / RAND_MAX) * ((float)rand_r(&seed) / RAND_MAX);
            B[i] = ((float)rand_r(&seed) / RAND_MAX) * ((float)rand_r(&seed) / RAND_MAX);
        }
        // pascal_stop(1);
        // pascal_start(2);
        for (int j = 0; j < n; j++)
#pragma omp parallel for
            for (int i = 0; i < n; i++)
                if (i <= j)
                {
                    U[i][j] = A[i][j];
                    for (int k = 0; k < i - 1; k++)
                        U[i][j] -= L[i][k] * U[k][j];
                    if (i == j)
                        L[i][j] = 1;
                    else
                        L[i][j] = 0;
                }
                else
                {
                    L[i][j] = A[i][j];
                    for (int k = 0; k <= j - 1; k++)
                        L[i][j] -= L[i][k] * U[k][j];
                    L[i][j] /= U[j][j];
                    U[i][j] = 0;
                }
        // printf("[L]: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < n; j++)
        //         printf("%9.3f", L[i][j]);
        //     printf("\n");
        // }
        // printf("\n\n[U]: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     for (int j = 0; j < n; j++)
        //         printf("%9.3f", U[i][j]);
        //     printf("\n");
        // }
        // pascal_stop(2);
        // pascal_start(3);
        for (int i = 0; i < n; i++)
        {
#pragma omp single
            Y[i] = B[i];
#pragma omp for reduction(- \
                          : Y[i])
            for (int j = 0; j < i; j++)
                Y[i] -= L[i][j] * Y[j];
        }
        // pascal_stop(3);
        // printf("\n\n[Y]: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     printf("%9.3f", Y[i]);
        // }
        // pascal_start(4);
        for (int i = n - 1; i >= 0; i--)
        {
#pragma omp single
            X[i] = Y[i];
#pragma omp for reduction(- \
                          : X[i])
            for (int j = i + 1; j < n; j++)
                X[i] -= U[i][j] * X[j];
#pragma omp single
            X[i] /= U[i][i];
        }
        // pascal_stop(4);
        // printf("\n\n[X]: \n");
        // for (int i = 0; i < n; i++)
        // {
        //     printf("%9.3f", X[i]);
        // }
        // printf("\n");
        float tmp;
        for (int i=0, tmp=0.0; i<n; i++){
            for(int j=0; j<n; j++)
                tmp += A[i][j] * X[j];
            if ((fabsf(tmp-B[i]))>0.0001){
                printf("Resultado não está certo\n");
                exit(-1);
            }
        }
    }
}