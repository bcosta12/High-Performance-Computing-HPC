#include <stdio.h>
#include <mpi.h>
void main (int argc, char ** argv ) {
    MPI_Init( &argc, &argv );
    
    
    int N = 100000;
    double step;
    int i; 
    double x, pi, sum_final, sum = 0.0;
    step = 1.0/(double) N;
    x = 0.5 * step;
    
    int rank;
    int np;
    
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &np );
    
    
    int chunk = N/np;
    int rest  = N % np;
    int init, final;


    if( rank < rest ) {
        init = (chunk+1)*rank;
        final = init + chunk;
    } else {
        init = (chunk+1)*rest + chunk*(rank-rest);
        final = init + chunk - 1;
    }
    
    printf("rank = %d init = %d final = %d\n", rank, init, final ); 
  
    
    for (i = init;i <= final; i++){
        x = step * i + 0.5*step;
        sum += 4.0/(1.0+x*x);
    }
    
    
    
    MPI_Reduce(&sum, &sum_final, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    
    if( rank == 0 ) {
        pi = step * sum_final;    
        printf("%lf\n", pi );
    }
    
    MPI_Finalize();
}
