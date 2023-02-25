#include <omp.h>
#include <stdio.h>

int main()
{
    int data1, data2, result;
    int flag1 = 0, flag2 = 0;
    int iam;

    #pragma omp parallel sections  private(iam)
    {
        #pragma omp section
        {
            iam = omp_get_thread_num();
            printf("Thread %d, esperando primer dato: \n", iam);
            scanf("%d", &data1);
            #pragma omp flush(data1)
            flag1 = 1;
            #pragma omp flush(flag1)
            printf("Thread %d, esperando segundo dato: \n", iam);
            scanf("%d", &data2);
            #pragma omp flush(data2)
            flag2 = 1;
            #pragma omp flush(flag2)
        }

        #pragma omp section
        {
            iam = omp_get_thread_num();
            while (!flag1 || !flag2)
            {
            #pragma omp flush(flag1, flag2)
            }
            #pragma omp flush(data1, data2)

            result = data1 + data2;
        }

        #pragma omp section
        {
            iam = omp_get_thread_num();
            while (!flag1 || !flag2 || !result)
            {
            #pragma omp flush(flag1, flag2)
            }
            #pragma omp flush(data1, data2)
            printf("Thread %d, resultado = %d \n", iam, result);
        }
    } // parallel
}
