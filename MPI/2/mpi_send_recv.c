#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main( int argc , char ** argv ){

	MPI_Init(&argc,&argv);

	int N = 12, rank, np, i;

	MPI_Comm_rank( MPI_COMM_WORLD , &rank );
	MPI_Comm_size( MPI_COMM_WORLD , &np );

	int chunk = N/np, init = chunk * rank, final = init + chunk;

	/*MPI_Send( void * buff, int count , MPI_Datatype d, int dest ,int tag, MPI_COMM_WORLD );

	if( rank % 2 == 0 ){
		MPI_Send( &rank , 1 , MPI_INT , rank + 1 , 0 , MPI_COMM_WORLD );
	}else{
		int r;
		MPI_Recv( &r , 1 , MPI_INT , rank - 1 , 0 , MPI_COMM_WORLD , NULL );
		printf("Eu rank %d , recebi a mensgem do rank %d\n " , rank , r );
	}*/

	float sum = 0.0;
	float * v1 = (float *) malloc ( sizeof( float ) * N );
	float * v2 = (float *) malloc ( sizeof( float ) * N );

	for ( i = 0 ; i < N ; i++){
		v2[i] = 1;
		v1[i] = 1;
	}

	for( i = init ; i < final ; i++ ){
		sum += v1[i]*v2[i];
	}

	if ( rank != 0 ){
		MPI_Send( &sum , 1 , MPI_FLOAT , 0 , 0 , MPI_COMM_WORLD );
	}else {
		float r;
		for( i = 1 ; i < np ; i++ ){
			MPI_Recv( &r , 1 , MPI_FLOAT , i , 0 , MPI_COMM_WORLD , NULL );
			sum += r;
		}
		printf("Dot product = %.2f\n" , sum );
	}

	MPI_Finalize();

	return 0;
}
