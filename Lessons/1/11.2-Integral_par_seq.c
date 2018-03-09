#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv){
	int NUM_STEPS = atoi(argv[1]), i;
	double t = 0, sum = 0.0, step = 1.0/NUM_STEPS;

	#pragma omp parallel private (i, t)
	{
		#pragma omp for reduction(+:sum)
		for(i = 0; i<NUM_STEPS;i++){
			t = (i+0.5)*step;
			sum += 4.0 / (1+t*t);
		}
	}
	double pi = step * sum;
	printf(" pi = %.10lf\n", pi);
}