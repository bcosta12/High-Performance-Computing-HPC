#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id;
	printf("Sequencial\n");
	#pragma omp parallel
	{
		printf("Parallel\n");
	}
}