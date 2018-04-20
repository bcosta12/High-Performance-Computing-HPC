#include <stdio.h>
#include <omp.h>
#include <math.h>


double f(double t);

int main (int argc, char ** argv) {
	//seq area
	int i;
	double s, t;
	int n = atoi( argv[1] );

	#pragma omp parallel reduction(+:s)
	{	s =0;
		#pragma omp for 
		for(int i = 0; i < n; ++i){ 
//			printf("%lf\n", (double)(i / n));
			t = ((double) i )/ n;
			s += f( t );
		}	

	}
	printf("s = %.4lf\n", s*(4.0/n));
}

double f( double t){
	
	return sqrt(1.0-(t*t));
}
