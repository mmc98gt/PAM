#include <stdio.h>
#include <omp.h>
int main()  
{                                 
	int iam =0, np = 1, i=0;     
	
	omp_set_num_threads(8);
	
	#pragma omp parallel private(iam, np,i)
    	{
		#if defined (_OPENMP) 
      		  np = omp_get_num_threads(); 
      		  iam = omp_get_thread_num();
                #endif
		printf("Hello from thread %d out of %d \n",iam,np);

		#pragma omp for 
		for(i=0;i<(np*2);i++)
		{
		  printf("Thread %d, contador %d \n",iam,i);
                }
    	
    }
}

