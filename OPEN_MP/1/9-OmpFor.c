#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	int v1[10];
	int v2[10];
	int v3[10];
	int i;
	
	for ( i = 0; i < 10 ; i++){
		v1[i] =i;
		v2[i] =i;
	}
	omp_set_num_threads( 4 ); 


	#pragma omp parallel
	{
		#pragma omp for 
		for(int i = 0; i < 10; ++i){
			v3[i] = v1[i]+v2[i];
			//what thread execute each iteration ?
			printf("i=%d thread-id=%d\n", i,omp_get_thread_num());

		}
	}	
	for(int i = 0; i < 10; ++i)
			printf("v3[%d] = %d\n", i, v3[i]);
}