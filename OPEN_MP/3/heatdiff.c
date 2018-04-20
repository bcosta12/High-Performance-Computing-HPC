#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include "color.h"

#define DEBUG 0
#define THRESHOLD (5e-3)

void print_matrix( double *phi, int n )
{
    int i, j;
    for ( i = 0; i < n; i++ )
    {
        printf("[" );
        for ( j = 0; j < n; j++ )
            printf(" %10.6f ", phi[j*n + i] );
        printf("]\n" );
    }
}

void init_phi_seq( double * phi, int n )
{
    int i, j;

    // Interior points initialized to 50 degrees
    for ( i = 1; i < n-1; i++ )
        for ( j = 1; j < n-1; j++ )
            phi[j*n+i] = 50.0;

    // Top, left, and right boundaries fixed at 100 degrees
    for ( i = 0; i < n; i++ )
    {
        phi[    0*n +    i] = 100.0;
        phi[(n-1)*n +    i] = 100.0;
        phi[    i*n +    0] = 100.0;
    }
    // Bottom boundary fixed at 0 degrees
    for ( i = 0; i < n; i++ )
        phi[    i*n +(n-1)] = 0.0;
}

void init_phi_par( double * phi, int n )
{
    int i, j;

    // Interior points initialized to 50 degrees
    #pragma omp parallel private ( i,j )
	    for ( i = 1; i < n-1; i++ )
	        for ( j = 1; j < n-1; j++ )
	            phi[j*n+i] = 50.0;

    // Top, left, and right boundaries fixed at 100 degrees
	#pragma omp parallel private ( i )        
	    for ( i = 0; i < n; i++ )
	    {
	        phi[    0*n +    i] = 100.0;
	        phi[(n-1)*n +    i] = 100.0;
	        phi[    i*n +    0] = 100.0;
	    }
    // Bottom boundary fixed at 0 degrees
	#pragma omp parallel private ( i )    
    	for ( i = 0; i < n; i++ )
            phi[    i*n +(n-1)] = 0.0;
}

void compute_seq( int n, int *niters )
{
    double *phi_cur, *phi_next, *tmp;
    double conv;
    int i, j;

    phi_cur  = (double *) malloc ( n * n * sizeof(double) );
    phi_next = (double *) malloc ( n * n * sizeof(double) );
    
    init_phi_seq(phi_cur, n);
    init_phi_seq(phi_next, n);

    *niters = 0;
    while( 1 )
    {
        (*niters)++;
#if DEBUG
        // printf("Iteration %d\n", *niters);
        // print_matrix( phi_cur, n );
        system("clear");
        print_colors( phi_cur, n );
        usleep(100000);
#endif
        // Compute next (new) phi from current (old) phi
        for( j = 1; j < n-1; j++ )
            for( i = 1; i < n-1; i++ )
                phi_next[ j*n+i ] = ( phi_cur[ (j-1)*n+i ] +
                                      phi_cur[ j*n+(i-1) ] +
                                      phi_cur[ j*n+(i+1) ] +
                                      phi_cur[ (j+1)*n+i ] ) / 4;
        // If converged, we are done
        conv = 1;
        for( j = 1; j < n-1; j++ )
            for( i = 1; i < n-1; i++ )
                if( fabs( phi_next[ j*n + i ] - phi_cur[ j*n + i ] ) > THRESHOLD )
                    conv = 0;
        if ( conv )
            break;
        // Otherwise, swap pointers and continue
        tmp = phi_cur;
        phi_cur = phi_next;
        phi_next = tmp;
    }

    free( phi_cur );
    free( phi_next );
}

void compute_par( int n, int *niters )
{
    double *phi_cur, *phi_next, *tmp;
    double conv;
    int i, j;

    phi_cur  = (double *) malloc ( n * n * sizeof(double) );
    phi_next = (double *) malloc ( n * n * sizeof(double) );
    
    init_phi_par(phi_cur, n);
    init_phi_par(phi_next, n);

    *niters = 0;
    while( 1 )
    {
        (*niters)++;
#if DEBUG
        // printf("Iteration %d\n", *niters);
        // print_matrix( phi_cur, n );
        system("clear");
        print_colors( phi_cur, n );
        usleep(100000);
#endif
        // Compute next (new) phi from current (old) phi
        #pragma omp parallel for private(j,i)
	        for( j = 1; j < n-1; j++ )
	            for( i = 1; i < n-1; i++ )
	                phi_next[ j*n+i ] = ( phi_cur[ (j-1)*n+i ] +
	                                      phi_cur[ j*n+(i-1) ] +
	                                      phi_cur[ j*n+(i+1) ] +
	                                      phi_cur[ (j+1)*n+i ] ) / 4;
	        // If converged, we are done
        conv = 1;
        #pragma omp parallel for private( i,j ) reduction ( *:conv )
	        for( j = 1; j < n-1; j++ )
	            for( i = 1; i < n-1; i++ )
	                if( fabs( phi_next[ j*n + i ] - phi_cur[ j*n + i ] ) > THRESHOLD )
	                    	conv = 0;
        if ( conv )	
            break;
        // Otherwise, swap pointers and continue
        tmp = phi_cur;
        phi_cur = phi_next;
        phi_next = tmp;
    }

    free( phi_cur );
    free( phi_next );
}

int main( int argc, char *argv[] ) 
{
    int n, niters;
    double t0, t1, tt;

    if (argc != 2)
    {
        fprintf( stderr, "Usage: %s <n>\n", argv[0] );
        exit( -1 );
    }

    n = atoi( argv[1] );

    printf("Version             | #iters | #threads | #Time (s) | Speedup \n");
    printf("------------------------------------------------------------- \n");
    
    t0 = omp_get_wtime();
    compute_seq( n, &niters );
    t1 = omp_get_wtime();
    tt = t1 - t0;
    printf("Sequential          | %6d | %8d | %9.2f | %7.2f \n", niters, 1, tt, 1.0);

    // Add your code here!

    printf("------------------------------------------------------------- \n");
	
    t0 = omp_get_wtime();
    compute_par( n, &niters );
    t1 = omp_get_wtime();
    tt = t1 - t0;
    printf("Parallel          | %6d | %8d | %9.2f | %7.2f \n", niters, 1, tt, 1.0);

    // Add your code here!

    printf("------------------------------------------------------------- \n");
	

    return 0;
}

