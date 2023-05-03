#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
    unsigned long i, h = 0, n = strtol(argv[1], NULL, 10);
    double x, y;
    srand(clock());

    for (i = 0; i < n; i++)
    {
        x = ((double)rand()) / RAND_MAX;
        y = ((double)rand()) / RAND_MAX;
        if (sqrt(x * x + y * y) <= 1.0)
            h++;
    }
    printf("Pi: %1.15f\n", 4.0 * h / n);
}