#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

float vet_mean( float *v, int size ){

	float mean_value, sum=0;
	int i;

	for (i = 0; i < size; i++ ){
		sum += v[i];
	}

	mean_value = sum / size;

	return mean_value;
}

int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	srand(time(NULL));

	int rank, np, i;
	float * vet_recv, * vet_send, * vet_final, partial_mean, mean;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if ( rank == 0 ) {
 		vet_send = (float *) malloc ( sizeof(float) * 128 * np );
 		for (i = 0; i < 128*np; i++)
 			vet_send[i] = rand()%32;
 	}

 	vet_recv = (float *) malloc ( sizeof(float) * 128 );

 	MPI_Scatter( vet_send, 128, MPI_FLOAT, vet_recv, 128, MPI_FLOAT, 0, MPI_COMM_WORLD);

 	partial_mean = vet_mean( vet_recv, 128 );

 	if (rank == 0)
		vet_final = (float *) malloc ( sizeof(float) * 128 );

	MPI_Gather( &partial_mean, 1, MPI_FLOAT, vet_final, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);


	if ( rank == 0 ) {
		mean = vet_mean( vet_final, np );
	 	printf("mean=%f\n", mean);
	}

	MPI_Finalize();
	return 0;
}
