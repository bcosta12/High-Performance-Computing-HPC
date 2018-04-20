#include <stdio.h>
#include <omp.h>

int main () {
	//seq area
	int id = 10;
	int s = 0;
	omp_set_num_threads( 4 ); 


	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				printf("Sec 1, id = %d\n", omp_get_thread_num());
			}
			#pragma omp section
			{
				printf("Sec 2, id = %d\n", omp_get_thread_num());
			}
			#pragma omp section
			{
				printf("Sec 3, id = %d\n", omp_get_thread_num());
			}
			#pragma omp section
			{
				printf("Sec 4, id = %d\n", omp_get_thread_num());
			}
		}
	}	
}