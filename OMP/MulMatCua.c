#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

int THREADS = 4;

void matrizCuadrada(double *m1, double *m2, double *mr, int tamaño, int filas,int hilos)
{
    int i, j, k, iam, nprocs;
    double suma;
    omp_set_num_threads(hilos);
    #pragma omp parallel private(iam, nprocs)
    {
        nprocs = hilos;
        iam = omp_get_thread_num();
        #pragma omp master
        THREADS = nprocs;
        #pragma omp for private(i, suma, j) schedule(dynamic, filas)
        for (i = 0; i < tamaño; i++)
        {
            #ifdef DEBUG
            printf("thread %d fila %d \n", iam, i);
            #endif
            suma = 0.;
            for (j = 0; j < tamaño; j++)
            {
                for (k = 0; k < tamaño; k++)
                    suma += m1[i * tamaño + k] * m2[j + k * tamaño];
                mr[i * tamaño + j] = suma;
                suma = 0.;
            }
        }
    }
}

void initialize(double *m, int t)
{
    int i;
    for (i = 0; i < t; i++)
        m[i] = (double)(i);
}

void initializealea(double *m, int t)
{
    int i;
    for (i = 0; i < t; i++)
        m[i] = (double)rand() / RAND_MAX;
}

void escribir(double *m, int t)
{
    int i, j;
    for (i = 0; i < t; i++)
    {
        for (j = 0; j < t; j++)
            printf("%.4lf ", m[i * t + j]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{

    double *m1, *m2, *mr; // matrices 1 y 2 a multiplicar, y mr es la matriz donde se guardará el resultado
    int tamaño = 0;
    int filas = 0;
    int hilos = 0; 
    double start, fin, tiempo, Mflops;

    if (argc < 4)
    {
        printf("\n\n USO %s tamanyo_matriz n_filas_consecutivas n_hilos \n\n", argv[0]);
        return -1;
    }

    tamaño = atoi(argv[1]);
    filas = atoi(argv[2]);
    hilos = atoi(argv[3]);

    // inicializar matrices
    m1 = (double *)malloc(sizeof(double) * tamaño * tamaño); // matriz 1
    m2 = (double *)malloc(sizeof(double) * tamaño * tamaño); // matriz 2
    mr = (double *)malloc(sizeof(double) * tamaño * tamaño); // matriz resultado
    initializealea(m1, tamaño * tamaño);
    initializealea(m2, tamaño * tamaño);

    start = omp_get_wtime();
    matrizCuadrada(m1, m2, mr, tamaño, filas, hilos);
    fin = omp_get_wtime();
    tiempo = fin - start;
    if (tiempo == 0.)
    {
        printf("No hay suficiente precision\n");
    }
    else
    {
        Mflops = ((2. * tamaño * tamaño) / tiempo) / 1000000.;
        printf("  threads %d, tamano %d\n    segundos: %.6lf, Mflops: %.6lf, Mflops por thread: %.6lf\n", THREADS, tamaño, tiempo, Mflops, Mflops / THREADS);
        printf("  Precision omp_get_wtick: numero de segundos entre sucesivos ticks de reloj usados por wtime=%lf \n", omp_get_wtick());
    }

    free(m1);
    free(m2);
    free(mr);
}