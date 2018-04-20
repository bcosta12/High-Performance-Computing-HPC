#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	int s = 0;
	omp_set_num_threads( 4 ); 


	#pragma omp parallel firstprivate(id) shared(s)
	{	
		//critical to take care about mutal exclusion 
		#pragma omp critical
		{
			s += 2;
			printf("Parallel, s = %d\n", s);
		}
	}
	//join
}