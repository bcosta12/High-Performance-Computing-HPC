#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

int isPrime( int n ){
	int sq = sqrt( n );
	int i = 2;
	while( i <= sq ) {
		if ( n % i == 0 )
			return 0;
		i++;
	}
	return 1;
}

int main(int argc, char **argv){

	int world_size;
	int rank, np;
	int i, j, k, l = 1;
	int final_number_global = atoi( argv[1] );
	int final_number = 1000;
	int chunk;
	int vector_to_send[2];
	int vector_to_rec[4];
	int total = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	//chunk = final_number / ( np - 1 );
	chunk = 1000;

	while( l*1000 <= final_number_global ){
		if ( rank == 0 ) {
			// Master
			for( i = 1; i < np ; i++ ){
				vector_to_send[0] = chunk*( l - 1 ) + 1; //start number
				vector_to_send[1] = vector_to_send[0] + chunk - 1 ; //final number
				MPI_Send ( &vector_to_send, 2, MPI_INT, i, 0, MPI_COMM_WORLD );
				l++;
			}
			for( i = 1; i < np ; i++ ){
				int recv;
				MPI_Recv( &recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL );
				vector_to_rec[i] = recv;
				total += recv;
			}


		} else{
			MPI_Recv( vector_to_rec, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL );
			int start = vector_to_rec[0];
			int final =	vector_to_rec[1];
			int count = 0;
			for(j = start; j < final; j ++){
				if ( isPrime( j ) ) count ++;
			}
			MPI_Send ( &count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD );
			l += (np-1);
		}

	}
	if( rank == 0 )
		printf("%d to %d - %d \n", 0, final_number_global, total);
	MPI_Finalize();

	return 0;
}
