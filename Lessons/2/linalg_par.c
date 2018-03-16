#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <sys/time.h>


double dot_prod_par( double * v1, double * v2, int size ) {
    double dot = 0.0;
    int i;

    #pragma omp parallel for reduction(+:dot) private(i)
    
        for(i=0; i < size; i++ ) {
            dot += v1[i]*v2[i];
        }
    

    return dot;
}

double dot_prod( double * v1, double * v2, int size ) {
    double dot = 0.0;
    int i;
    for(i=0; i < size; i++ ) {
        dot += v1[i]*v2[i];
    }
    return dot;
}

void mat_vec_mul_par( double * r, double ** m, double * v, int nlines, int ncols ) {
    double dot = 0.0;
    int i,j;

    #pragma omp parallel for private(dot, j)
        for(i=0; i < nlines; i++ ) {
            dot = 0.0;
            for(j=0; j < ncols; j++ ) {
                dot += m[i][j]*v[j];      
            }
            r[i] = dot;
        }
}

void mat_vec_mul( double * r, double ** m, double * v, int nlines, int ncols ) {
    double dot = 0.0;
    int i,j;
    for(i=0; i < nlines; i++ ) {
        dot = 0.0;
        for(j=0; j < ncols; j++ ) {
            dot += m[i][j]*v[j];      
        }
        r[i] = dot;
    }
}

void mat_mul_par( double ** r, double ** m1, double ** m2, int nlines_m1, int cols_m1, int ncols_m2 ) {
    double dot = 0.0;
    int i,j,k;

     #pragma omp parallel for private(dot, j, k)
    for(i=0; i < nlines_m1; i++ ) {
        for(j=0; j < ncols_m2; j++ ) {
            dot = 0.0;
            for( k=0; k < ncols_m2; k++) {
                dot += m1[i][k]*m2[k][j];      
            }
            r[i][j] = dot;
        }
    }
}

void mat_mul( double ** r, double ** m1, double ** m2, int nlines_m1, int cols_m1, int ncols_m2 ) {
    double dot = 0.0;
    int i,j,k;
    for(i=0; i < nlines_m1; i++ ) {
        for(j=0; j < ncols_m2; j++ ) {
            dot = 0.0;
            for( k=0; k < ncols_m2; k++) {
                dot += m1[i][k]*m2[k][j];      
            }
            r[i][j] = dot;
        }
    }
}

void print_vet( double * r, int size ) {
    int i;
    for(i=0; i < size; i++ ) {
        printf("%lf\n", r[i] );
    }
    printf("\n");
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

double * alloc_init_vet(double value, int size ) {
    int i;
    double * r = (double *) malloc( sizeof( double  ) * size );
    for(i=0; i < size; i++ ) {
        r[i] = value;
    }
    return r;
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


double * alloc_vet( int size ) {
    int i;
    double * r = (double *) malloc( sizeof( double  ) * size );
    return r;
}

double ** alloc_mat( double ** m, int nlines, int ncols ) {
    int i, j;
    double ** mat = (double **) malloc( sizeof( double * ) * nlines );
    for(i=0; i < nlines; i++ ) {
        mat[i] = (double *)malloc( sizeof( double ) * ncols );
    }
    return mat;
}

long time_usec( struct timeval t0, struct timeval t1 ){
    return ( t1.tv_sec - t0.tv_sec ) * 1000 * 1000 + ( t1.tv_usec - t0.tv_usec );
}

int main() {
    
    struct timeval t0,t1;

    int N = 2000;
  
    double ** m = alloc_init_mat( 1.0, N, N );
    double ** r = alloc_init_mat( 1.0, N, N );
   
    

    gettimeofday( &t0, NULL );
    mat_mul_par( r, m, m, N, N, N);
    gettimeofday( &t1, NULL );

    long t = time_usec( t0, t1 );

    printf("time [P]: %ld\n", t);
    

    gettimeofday( &t0, NULL );
    mat_mul( r, m, m, N, N, N );
    gettimeofday( &t1, NULL );

    t = time_usec( t0, t1 );

    printf("time [S]: %ld\n", t);
         
           
}

