#include <stdio.h>
#include <mpi.h>
#include <math.h>

int isPrime( int n ){
	int sq = sqrt( n );
	int i = 2;
	while( i <= sq ) {
		if ( n % i == 0 )
			return 0; //not prime
		i++;	
	}
	return 1; //prime
}

void getPrimes( int * p1, int * p2, int even){
	int i, j;
	for ( i = 2; i < even ; i++ ) {
		for ( j=2; j < even; j++ ){
			if ( isPrime(i) && isPrime(j) ){
				if (even == i+j){
					*p1 = i;
					*p2 = j;
					return;
				}			
			}		
		}
	}
}

int main(int argc, char **argv){

	int world_size;
	int rank, np;
	int i;

	MPI_Init(&argc, &argv);	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	if ( rank == 0 ) {
		// Master
		
		int even;
		for ( i = 1; i < np; i++){
			even = 2*( i+1 );
			MPI_Send ( &even, 1, MPI_INT, i, 0, MPI_COMM_WORLD );		
		}
		even = 2 * (np + 1);
		int buf[3];
		while( 1 ){
			MPI_Recv( buf, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
			printf(" [%d] %d + %d = %d \n", buf[2], buf[0], buf[1], buf[0]+ buf[1]);
			int rank_to_send = buf[2];
			MPI_Send ( &even, 1, MPI_INT, rank_to_send, 0, MPI_COMM_WORLD );
			even +=2;
		}
	} else {
		// Slave
		
		int even_recv, p1, p2;
		int buf_send[3];
		while( 1 ) {
			MPI_Recv( &even_recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL );
			getPrimes( &p1, &p2, even_recv);
			buf_send[0] = p1;
			buf_send[1] = p2;
			buf_send[2] = rank;
			MPI_Send ( buf_send, 3, MPI_INT, 0, 0, MPI_COMM_WORLD );	
		}	
	}

	MPI_Finalize();
	return 0;
}
