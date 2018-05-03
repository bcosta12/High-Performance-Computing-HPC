#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	int rank, np, i;
	int * vet_recv;
	int * vet_send;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (rank == 0)
		vet_recv = (int *) malloc ( sizeof(int) * 2 * np );

	vet_send = (int *) malloc ( sizeof(int) * 2 );
	vet_send[0] = rank;
	vet_send[1] = 2*rank;

	MPI_Gather( vet_send, 2, MPI_INT, vet_recv, 2, MPI_INT, 0, MPI_COMM_WORLD);

 	if ( rank == 0 ) {
 		for (i = 0; i < 2*np; i++)
			printf("vet_recv[%d]=%d\n", i, vet_recv[i]);
 	}
 	
	MPI_Finalize();
	return 0;
}
