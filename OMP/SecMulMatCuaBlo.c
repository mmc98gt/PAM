#include <stdio.h>
#include <stdlib.h>

void compare_matrices(double *A, double *B, int n)
{
    double tolerance = 1e-6;
    for (int i = 0; i < n; i++)
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
void multiply_block(double *A, double *B, double *C, int n, int i, int j, int k, int block_size)
{
    // Recorrer los elementos del bloque
    for (int ii = i; ii < i + block_size; ii++)
    {
        for (int jj = j; jj < j + block_size; jj++)
        {
            double sum = 0;
            for (int kk = k; kk < k + block_size; kk++)
            {
                sum += A[ii * n + kk] * B[kk * n + jj];
            }
            C[ii * n + jj] += sum;
        }
    }
}

// Función para multiplicar dos matrices utilizando un esquema por bloques
void multiply_matrices(double *A, double *B, double *C, int n, int block_size)
{
    // Multiplicar las matrices por bloques
    for (int i = 0; i < n; i += block_size)
    {
        for (int j = 0; j < n; j += block_size)
        {
            for (int k = 0; k < n; k += block_size)
            {
                // Multiplicar el bloque de las matrices
                multiply_block(A, B, C, n, i, j, k, block_size);
            }
        }
    }
}


void multiply_matrices_Seq(double *A, double *B, double *C, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(int argc, char *argv[])
{
    // Obtener la dimensión de la matriz y el tamaño del bloque a partir de los argumentos de la línea de comandos
    int n = atoi(argv[1]);
    int block_size = atoi(argv[2]);
    if(n%block_size!=0)
    {
        printf("error tamaño de bloque no multiplo de la matriz");
        return -1;
    }

    // Crear las matrices A, B y C
    double *A = (double *)malloc(n * n * sizeof(double));
    double *B = (double *)malloc(n * n * sizeof(double));
    double *C = (double *)calloc(n * n, sizeof(double));
    double *C2 = (double *)calloc(n * n, sizeof(double));

    // Inicializar las matrices A y B con valores aleatorios
    for (int i = 0; i < n * n; i++)
    {
        A[i] = (double)rand() / RAND_MAX;
        B[i] = (double)rand() / RAND_MAX;
    }

    // Multiplicar las matrices A y B y almacenar el resultado en C
    multiply_matrices(A, B, C, n, block_size);
    multiply_matrices_Seq(A, B, C2, n);
    compare_matrices(C, C2, n);

    // Liberar la memoria
    free(A);
    free(B);
    free(C);
    free(C2);

    return 0;
}
