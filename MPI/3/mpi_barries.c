#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);
	
	int rank, np;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
 	sleep( rank );
 	printf("Rank %d wake up\n", rank);
 	MPI_Barrier( MPI_COMM_WORLD );
	printf("Rank %d\n", rank);

	MPI_Finalize();
	return 0;
}
