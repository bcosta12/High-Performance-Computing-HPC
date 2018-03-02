#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	int s = 0;
	omp_set_num_threads( 4 ); 


	#pragma omp parallel firstprivate(id) shared(s)
	{	
		//s is a shared variable
		//all thread will increment s
		//WARN: OMP Does not take care about mutual exclusion
		s += 2;	
		
		printf("Parallel, s = %d\n", s);
	}
	//join
}