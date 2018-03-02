#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	
	omp_set_num_threads( 4 ); 


	#pragma omp parallel private(id)
	{	
		id = 2 + omp_get_thread_num();	
		//each thread has one private id, copied in memory 
		printf("Parallel, id = %d\n", id);
	}
	//join
}