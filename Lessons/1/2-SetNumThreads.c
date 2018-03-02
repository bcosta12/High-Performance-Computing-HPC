#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id;
	
	omp_set_num_threads( 2 ); 
	
	#pragma omp parallel
	{
		printf("Parallel\n");
	}
}