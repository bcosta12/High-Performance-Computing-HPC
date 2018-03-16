#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>

void gausselim( double ** m, int nlines_m ){

	int j, i, k;
	double pivo;
	for( j = 0; j<nlines_m; j++ ){
		for( i = j+1; i<nlines_m ; i++){
			pivo = (m[i][j]/m[j][j]);
			for(k = j; k <nlines_m ; k++){
				m[i][k] = m[i][k] - pivo*m[j][k];
			}
		}
	}
}

double ** alloc_init_mat( double value, int nlines, int ncols ) {
    int i, j;
    double ** mat = (double **) malloc( sizeof( double * ) * nlines );
    for(i=0; i < nlines; i++ ) {
        mat[i] = (double *)malloc( sizeof( double ) * ncols );
        for(j=0; j < ncols; j++ ) {
            mat[i][j] = value;
        }
    }
    return mat;
}

void print_mat( double ** m, int nlines, int ncols ) {
    int i, j;
    for(i=0; i < nlines; i++ ) {
        for(j=0; j < ncols; j++ ) {
            printf("%.4lf ", m[i][j] );
        }
        printf("\n");
    }
    printf("\n");
}

long time_usec( struct timeval t0, struct timeval t1 ){
    return ( t1.tv_sec - t0.tv_sec ) * 1000 * 1000 + ( t1.tv_usec - t0.tv_usec );
}

int main () {

	int N = 2;
	double  ** m = alloc_init_mat(1.0, N, N);
	struct timeval t0,t1;
    
    gettimeofday( &t0, NULL );
    gausselim(m, N);
    gettimeofday( &t1, NULL );

    long t = time_usec( t0, t1 );

    printf("time [S]: %ld\n", t);
   	print_mat(m, N, N);
}