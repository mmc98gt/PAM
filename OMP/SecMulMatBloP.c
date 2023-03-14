#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void compare_matrices(double *A, double *B, int m, int n)
{
    double tolerance = 1e-6;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (abs(A[i * n + j] - B[i * n + j]) > tolerance)
            {
                printf("Error en la posición (%d,%d): %f != %f\n", i, j, A[i * n + j], B[i * n + j]);
                return;
            }
        }
    }
    printf("Los resultados son iguales\n");
}

// Función para multiplicar un bloque de matrices
void multiply_block(double *A, double *B, double *C, int m, int n, int k, int i, int j, int sub_k, int block_size)
{
    // Recorrer los elementos del bloque
    for (int ii = i; ii < i + block_size; ii++)
    {
        for (int jj = j; jj < j + block_size; jj++)
        {
            double sum = 0;
            for (int kk = sub_k; kk < sub_k + block_size; kk++)
            {
                sum += A[ii * k + kk] * B[kk * n + jj];
            }
            C[ii * n + jj] += sum;
        }
    }
}

// Función para multiplicar dos matrices utilizando un esquema por bloques
void multiply_matrices(double *A, double *B, double *C, int m, int n, int k, int block_size, int hilos)
{
    clock_t start = clock(); // Tiempo de inicio
    // Multiplicar las matrices por bloques
    double start_time = omp_get_wtime(); // Tiempo de inicio
    int i, j, sub_k;
    #pragma omp parallel for collapse(3) schedule(static) num_threads(hilos)
    for ( i = 0; i < m; i += block_size)
    {
        for ( j = 0; j < n; j += block_size)
        {
            for ( sub_k = 0; sub_k < k; sub_k += block_size)
            {
                // Multiplicar el bloque de las matrices
                multiply_block(A, B, C, m, n, k, i, j, sub_k, block_size);
            }
        }
    }
    #pragma omp barrier
    double end_time = omp_get_wtime(); // Tiempo de fin
    double time_spent = (end_time - start_time) * 1000; // Tiempo transcurrido en milisegundos
    printf("Tiempo de ejecución: %f ms\n", time_spent);
}

void multiply_matrices_seq(double *A, double *B, double *C, int m, int n, int k)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double sum = 0;
            for (int l = 0; l < k; l++)
            {
                sum += A[i * k + l] * B[l * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(int argc, char *argv[])
{
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    int block_size = atoi(argv[4]);
    int hilos = 1;
    hilos = atoi(argv[5]);

    // Verificar que las dimensiones de las matrices sean múltiplos del tamaño del bloque
    if (m % block_size != 0 || n % block_size != 0 || k % block_size != 0)
    {
        fprintf(stderr, "Error: las dimensiones de las matrices deben ser múltiplos del tamaño del bloque.\n");
        exit(EXIT_FAILURE);
    }
    double *A = (double *)malloc(m * k * sizeof(double));
    double *B = (double *)malloc(k * n * sizeof(double));
    double *C = (double *)calloc(m * n, sizeof(double));
    double *C_seq = (double *)calloc(m * n, sizeof(double));

    // Inicializar las matrices A y B con valores aleatorios
    srand(0);
    for (int i = 0; i < m * k; i++)
    {
        A[i] = (double)rand() / RAND_MAX;
    }
    for (int i = 0; i < k * n; i++)
    {
        B[i] = (double)rand() / RAND_MAX;
    }

    // Multiplicar las matrices A y B y almacenar el resultado en C
    multiply_matrices(A, B, C, m, n, k, block_size, hilos);
    multiply_matrices_seq(A, B, C_seq, m, n, k);
    compare_matrices(C, C_seq, m, n);

    // Liberar la memoria
    free(A);
    free(B);
    free(C);
    free(C_seq);

    return 0;
}