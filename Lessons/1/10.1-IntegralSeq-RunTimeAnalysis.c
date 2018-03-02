#include <stdio.h>
#include <omp.h>
#include <math.h>


double f(double t);

int main (int argc, char ** argv) {
	//seq area
	int i;
	double s, t;
	int n = atoi( argv[1] );


	s = t = 0;

	for(i = 0; i< n; i++){
		s += f(t);
		t += 1.0/n;
	}
	printf("s = %.4lf\n", s*(4.0/n));
}

double f(double t){
	return sqrt(1.0-(t*t));
}