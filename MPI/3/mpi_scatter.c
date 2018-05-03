#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv){

	MPI_Init(&argc, &argv);

	int rank, np;
	int * vet_send;
	int * vet_recv;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

 	if ( rank == 0 ) {
 		int i;
 		vet_send = (int *) malloc ( sizeof(int) * 4 * np );
 		for (i = 0; i < 4*np; i++)
 			vet_send[i] = i;
 	}

 	vet_recv = (int *) malloc ( sizeof(int) * 4 );

 	MPI_Scatter( vet_send, 4, MPI_INT, vet_recv, 4, MPI_INT, 0, MPI_COMM_WORLD);

	//choose a rank to print, or print all of them
 	//if(rank == 1){
		int i;
 		for (i = 0; i < 4; i++)
			printf("vet_recv[%d]=%d rank %d\n", i, vet_recv[i], rank);
 	//}

 	if( rank == 0 )
 		free( vet_send );
 	free( vet_recv );

	MPI_Finalize();
	return 0;
}
