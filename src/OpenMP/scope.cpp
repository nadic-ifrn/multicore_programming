#include <omp.h>
#include <iostream>

int main() {
    int a = 10;
    int b = 20;
    int c = 0;
    int tid;

    #pragma omp parallel private(tid) firstprivate(a) shared(b, c)
    {
        tid = omp_get_thread_num();

        #pragma omp critical
        {
            a += tid;
            b += tid;
            c += tid;
            std::cout << "Thread " << tid << " tem a = " << a << ", b = " << b << ", c = " << c << std::endl;
        }
    }

    std::cout << "Após a região paralela, a = " << a << ", b = " << b << ", c = " << c << std::endl;

    return 0;
}
