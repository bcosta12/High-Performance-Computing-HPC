#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	int s = 0;
	omp_set_num_threads( 4 ); 


	#pragma omp parallel reduction(+:s)
	{
		s = 2;
	}
	//sum all s threads
	//time: log2(n)
	printf("FINAL s = %d\n", s);
	//join
}