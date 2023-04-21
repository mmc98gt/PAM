#include <omp.h>
int main()  
{                                 
  int iam =0, np = 1;     
  
  
      	printf("Maximum num of processors in this platform for this program: %d\n",omp_get_num_procs());
  
	#pragma omp parallel private(iam, np)
    {
			#if defined (_OPENMP) 
      		np = omp_get_num_threads(); 
      		iam = omp_get_thread_num();
			#endif
    	printf("Hello from thread %d out of %d \n",iam,np);
    }
}

