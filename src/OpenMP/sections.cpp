#include <omp.h>
#include <iostream>

void heavy_computation() {
    double res = 0;
    for (int i = 0; i < 1e6; ++i) {
        res += i / (i + 1.0);
    }
    std::cout << "Resultado do cálculo: " << res << ", realizado pela thread " << omp_get_thread_num() << std::endl;
}

int main() {
    omp_set_num_threads(6);
    #pragma omp parallel 
    {
        // Exemplo do uso de omp sections
        #pragma omp sections
        {
            #pragma omp section
            {
                std::cout << "Seção 1 sendo executada pela thread " << omp_get_thread_num() << std::endl;
                heavy_computation();
            }

            #pragma omp section
            {
                std::cout << "Seção 2 sendo executada pela thread " << omp_get_thread_num() << std::endl;
                heavy_computation();
            }
        }

        // Exemplo do uso de omp master
        #pragma omp master 
        {
            std::cout << "Esta parte do código é executada apenas pela thread mestra (thread " << omp_get_thread_num() << ")" << std::endl;
            heavy_computation();
        }

        // Exemplo do uso de omp single
        #pragma omp single 
        {
            std::cout << "Esta parte do código é executada por uma única thread (neste caso, a thread " << omp_get_thread_num() << ")" << std::endl;
            heavy_computation();
        }
    }
    return 0;
}
