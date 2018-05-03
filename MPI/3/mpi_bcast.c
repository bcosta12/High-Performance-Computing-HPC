#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	int rank;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 	if ( rank == 0 )
 		pi = 3.14156591;

 	MPI_Bcast( &pi, 1, MPI_FLOAT, 0, MPI_COMM_WORLD );

	printf("pi %f\nrank %d\n", pi, rank);

	MPI_Finalize();
	return 0;
}
