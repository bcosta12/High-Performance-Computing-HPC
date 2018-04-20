#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id;
	
	omp_set_num_threads( 8 ); 
	
	#pragma omp parallel
	{		
		printf("Parallel, id = %d\n", omp_get_thread_num());
	}
	//join
}