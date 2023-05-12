#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef vector<double> Vector;
typedef vector<Vector> Matrix;

// Função para realizar a multiplicação matriz-vetor
Vector matVecMult(const Matrix &A, const Vector &x) {
    int n = A.size();
    Vector result(n, 0);
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += A[i][j] * x[j];
        }
    }
    return result;
}

// Função para calcular o produto escalar de dois vetores
double dotProduct(const Vector &a, const Vector &b) {
    int n = a.size();
    double result = 0;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < n; ++i) {
        result += a[i] * b[i];
    }
    return result;
}

// Função para subtrair dois vetores
Vector vecSubtract(const Vector &a, const Vector &b) {
    int n = a.size();
    Vector result(n);
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}

// Função para realizar a multiplicação escalar-vetor
Vector scalarVecMult(double scalar, const Vector &vec) {
    int n = vec.size();
    Vector result(n);
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        result[i] = scalar * vec[i];
    }
    return result;
}

// Método Gradiente Conjugado
Vector conjugateGradient(const Matrix &A, const Vector &b, int max_iter, double tol) {
    
    int n = A.size();
    Vector x(n, 0); // Inicializa o vetor solução com zeros
    Vector r = vecSubtract(b, matVecMult(A, x)); // Resíduo inicial
    Vector p = r;
    double r_dot = dotProduct(r, r);
    int k;
    for (k = 0; k < max_iter; ++k) {
        Vector Ap = matVecMult(A, p);
        double alpha = r_dot / dotProduct(p, Ap);
        x = vecSubtract(x, scalarVecMult(alpha, p));
        r = vecSubtract(r, scalarVecMult(alpha, Ap));

        double r_dot_new = dotProduct(r, r);
        if (sqrt(r_dot_new) < tol) {
            break;
        }

        double beta = r_dot_new / r_dot;
        p = vecSubtract(r, scalarVecMult(beta, p));

        r_dot = r_dot_new;
    }

    cout << "Finished with " << k << " iterations" << endl;

    return x;
}

// Função para gerar um número double aleatório entre min e max
double randDouble(double min, double max, unsigned int *seed) {
    double randNum = static_cast<double>(rand_r(seed)) / static_cast<double>(RAND_MAX);
    return min + randNum * (max - min);
}

// Função para gerar uma matriz A de tamanho n x n e vetor B de tamanho n
void generateSystem(int n, Matrix &A, Vector &B, double sparsityFactor) {
    A.resize(n, Vector(n));
    B.resize(n);
    
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        double rowSum = 0;
        for (int j = 0; j < n; ++j) {
            // Utilizamos um número constante em vez de um número aleatório
            double num = (i + j) % 2 == 0 ? 0 : 1;

            if (num > sparsityFactor) {
                A[i][j] = 1 + i % 10; // Números determinísticos em vez de aleatórios
                rowSum += abs(A[i][j]);
            } else {
                A[i][j] = 0;
            }
        }
        // Tornamos a matriz diagonalmente dominante
        A[i][i] = rowSum + 1 + i % 10; // Números determinísticos em vez de aleatórios
        B[i] = 1 + i % 10; // Números determinísticos em vez de aleatórios
    }
}

// Função para gerar uma matriz A esparsa e diagonalmente dominante aleatória de tamanho n x n e vetor B de tamanho n
void generateRandomSystem(int n, Matrix &A, Vector &B, double sparsityFactor, unsigned int &seed) {
    A.resize(n, Vector(n));
    B.resize(n);

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        double rowSum = 0;
        for (int j = 0; j < n; ++j) {
            // Gera um número aleatório entre 0 e 1
            double randNum = static_cast<double>(rand_r(&seed)) / static_cast<double>(RAND_MAX);

            if (randNum > sparsityFactor) {
                A[i][j] = randDouble(1, 10, &seed);
                rowSum += abs(A[i][j]);
            } else {
                A[i][j] = 0;
            }
        }
        // Torna a matriz diagonalmente dominante
        A[i][i] = rowSum + randDouble(1, 10, &seed);
        B[i] = randDouble(1, 10, &seed);
    }
}

// Função para imprimir a matriz A e o vetor B
void printSystem(const Matrix &A, const Vector &B) {
    cout << "Matrix A:" << endl;
    for (const auto &row : A) {
        for (const auto &value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << "Vector B:" << endl;
    for (const auto &value : B) {
        cout << value << endl;
    }
}

int main(int argc, char **argv) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " <size> <sparsityFactor> <max_iter> <tol>" << endl;
        return 1;
    }

    // Define as variáveis de ambiente para afinidade de thread
    setenv("OMP_PROC_BIND", "true", 1);
    setenv("OMP_PLACES", "cores", 1);

    int size = stoi(argv[1]);
    double sparsityFactor = stod(argv[2]);
    int max_iter = stoi(argv[3]);
    double tol = stod(argv[4]);

    // Declara e inicializa a semente
    unsigned int seed = static_cast<unsigned int>(time(NULL));

    Matrix A;
    Vector B;

    //generateRandomSystem(size, A, B, sparsityFactor, seed);
    generateSystem(size, A, B, sparsityFactor);

    // Imprime a matriz A e o vetor B gerados
    //printSystem(A,B);
    double start, end;
    start = omp_get_wtime();
    Vector solution = conjugateGradient(A, B, max_iter, tol);
    cout << "Solution:" << endl;
    for (int i = 0; i < solution.size(); ++i) {
        cout << "x[" << i << "] = " << solution[i] << endl;
    }
   end = omp_get_wtime();
   printf("Tempo decorrido = %.16g segundos\n", end - start);

    return 0;
}
