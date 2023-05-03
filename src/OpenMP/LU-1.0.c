#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main(int argc, char **argv)
{
    int seed = clock(), n = strtol(argv[1], NULL, 10);
    float A[n][n], L[n][n], U[n][n], B[n], X[n], Y[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = ((float)rand_r(&seed) / RAND_MAX) * ((float)rand_r(&seed) / RAND_MAX);
    for (int i = 0; i < n; i++)
        B[i] = ((float)rand_r(&seed) / RAND_MAX) * ((float)rand_r(&seed) / RAND_MAX);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
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
    printf("[A]: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%9.3f", A[i][j]);
        printf("\n");
    }
    printf("\n[L]: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%9.3f", L[i][j]);
        printf("\n");
    }
    printf("\n[U]: \n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            printf("%9.3f", U[i][j]);
        printf("\n");
    }
    for (int i = 0; i < n; i++)
    {
        Y[i] = B[i];
        for (int j = 0; j < i; j++)
            Y[i] -= L[i][j] * Y[j];
    }
    printf("\n[B]: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%9.3f", B[i]);
    }
    printf("\n[Y]: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%9.3f", Y[i]);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        X[i] = Y[i];
        for (int j = i + 1; j < n; j++)
            X[i] -= U[i][j] * X[j];
        X[i] /= U[i][i];
    }
    printf("\n[X]: \n");
    for (int i = 0; i < n; i++)
    {
        printf("%9.3f", X[i]);
    }
    printf("\n");
    float tmp;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0, tmp = 0.0; j < n; j++)
            tmp += A[i][j] * X[j];
        if ((fabsf(tmp - B[i])) > 0.1)
        {
            printf("Resultado não está certo\n");
            exit(-1);
        }
    }
}